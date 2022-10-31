#include "rsp_actionhandler.h"
#include <QApplication>
#include <fstream>
#include <filesystem>
#include <random>

namespace fs = std::filesystem;

enum Pattern
{
    random,
    analyse_1
};

enum Result
{
    win,
    draw,
    lose
};

RSP_ActionHandler::RSP_ActionHandler(QObject *parent)
    : QObject{parent}, computers_pattern_(random)
{
    fs::path stat_dir = "statistics";
    if (!fs::exists(stat_dir))
        fs::create_directory(stat_dir);

    game_results_ = LoadResults();
    player_choices_ = LoadChoices("statistics/player_choices.bin");
    cpu_choices_ = LoadChoices("statistics/cpu_choices.bin");
}

void RSP_ActionHandler::aboutToQuit()
{
    ArrayToFile(game_results_, "statistics/results.bin");
    ArrayToFile(player_choices_, "statistics/player_choices.bin");
    ArrayToFile(cpu_choices_, "statistics/cpu_choices.bin");
}

void RSP_ActionHandler::onQuit()
{
    qApp->quit();
}

void RSP_ActionHandler::onSaveOptions(const bool option)
{
    if (option)
        computers_pattern_ = analyse_1;
    else
        computers_pattern_ = random;
}

void RSP_ActionHandler::updStatisticsReq()
{
    emit updStatisticsReply(game_results_[0], game_results_[1], game_results_[2]);
}

void RSP_ActionHandler::onGameButtonClicked(int symbol)
{
    char user_pick = symbol;
    user_pick_src_ = CharToSrc(user_pick);
    GameRound(user_pick);
    emit signalGameResult(user_pick_src_,sign_src_,comp_pick_src_,result_,color_);
}

std::array<int, 3> RSP_ActionHandler::LoadResults()
{
    std::array<int, 3> res = { 0 };
    std::fstream file("statistics/results.bin");
    if (file.is_open())
        for (auto& val : res)
            file.read(reinterpret_cast<char*>(&val), sizeof(val));

    return res;
}

std::vector<char> RSP_ActionHandler::LoadChoices(const char *file_name)
{
    std::vector<char> res;
    std::fstream file(file_name, std::ios_base::in);
    if (file.is_open())
    {
        char symbol = ' ';
        while (!file.eof())
        {
            file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
            res.push_back(symbol);
        }
    }
    return res;
}

void RSP_ActionHandler::GameRound(char choice)
{
    char cpu_choice = GetCpuChoice();
    comp_pick_src_ = CharToSrc(cpu_choice);
    int res = GetRoundResult(choice, cpu_choice);
    if (res == win)
    {
        ++game_results_[0];
        sign_src_ = CharToSrc('m');
        result_ = "WIN";
        color_ = "green";
    }
    else if (res == draw)
    {
        ++game_results_[1];
        sign_src_ = CharToSrc('e');
        result_ = "DRAW";
        color_ = "blue";
    }
    else if (res == lose)
    {
        ++game_results_[2];
        sign_src_ = CharToSrc('l');
        result_ = "LOSE";
        color_ = "red";
    }

    player_choices_.push_back(choice);
    cpu_choices_.push_back(cpu_choice);
}

char RSP_ActionHandler::GetCpuChoice()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    if (computers_pattern_ == analyse_1)
    {
        std::array<int, 3> rsp = { 0 };
        if (player_choices_.size() > 1)
        {
            char last_symbol = player_choices_[player_choices_.size() - 1];
            for (size_t i = 0; i < player_choices_.size() - 1; ++i)
            {
                if (player_choices_[i] == last_symbol)
                {
                    if (player_choices_[i + 1] == 'r')
                        ++rsp[0];
                    else if (player_choices_[i + 1] == 's')
                        ++rsp[1];
                    else if (player_choices_[i + 1] == 'p')
                        ++rsp[2];
                }
            }
        }

        int counter = 0;
        int max = 0;
        for (const auto& val : rsp)
            if (val > max)
            {
                max = val;
                counter = 1;
            }
            else if (val == max)
                ++counter;
        if (counter > 1)
        {
            std::uniform_int_distribution<> distrib(1, counter);
            int res = distrib(gen);
            for (size_t i = 0; i < rsp.size(); ++i)
                if (rsp[i] == max && res == 1)
                    return IntToChar(i);
                else if (rsp[i] == max && res != 1)
                    --res;
        }
        else
        {
            for (size_t i = 0; i < rsp.size(); ++i)
                if (rsp[i] == max)
                    return IntToChar(i);
        }
        return ' ';
    }
    else
    {
        std::uniform_int_distribution<> distrib(1, 3);
        int res = distrib(gen);
        if (res == 1)
            return 'r';
        else if (res == 2)
            return 's';
        else
            return 'p';
    }
}

char RSP_ActionHandler::IntToChar(int input)
{
    if (input == 0)
        return 'r';
    else if (input == 1)
        return 's';
    else
        return 'p';
}

int RSP_ActionHandler::GetRoundResult(char choice, char cpu_choice)
{
    if (choice == cpu_choice)
        return draw;
    else if (choice == 'r')
    {
        if (cpu_choice == 's')
            return win;
        else if (cpu_choice == 'p')
            return lose;
    }
    else if (choice == 's')
    {
        if (cpu_choice == 'p')
            return win;
        else if (cpu_choice == 'r')
            return lose;
    }
    else if (choice == 'p')
    {
        if (cpu_choice == 'r')
            return win;
        else if (cpu_choice == 's')
            return lose;
    }
    return 4;
}

QString RSP_ActionHandler::CharToSrc(char input)
{
    if (input == 'r')
        return "qrc:/res/rock.jpg";
    else if (input == 's')
        return "qrc:/res/scissors.jpg";
    else if (input == 'p')
        return "qrc:/res/paper.jpg";
    else if (input == 'm')
        return "qrc:/res/greater_than.png";
    else if (input == 'e')
        return "qrc:/res/equals.png";
    else if (input == 'l')
        return "qrc:/res/less_than.png";

    return "error";
}


template<typename T>
void RSP_ActionHandler::ArrayToFile(const T& array, const char* file_name)
{
    std::fstream file(file_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    for (const auto val : array)
        file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    file.close();
}
