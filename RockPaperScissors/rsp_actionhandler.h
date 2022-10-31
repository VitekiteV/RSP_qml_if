#ifndef RSP_ACTIONHANDLER_H
#define RSP_ACTIONHANDLER_H

#include <QObject>

class RSP_ActionHandler : public QObject
{
    Q_OBJECT
public:
    explicit RSP_ActionHandler(QObject *parent = nullptr);

public slots:
    void aboutToQuit();

private slots:
    void onQuit();
    void onSaveOptions(bool option);
    void updStatisticsReq();
    void onGameButtonClicked(int symbol);

signals:
    void updStatisticsReply(int W, int D, int L);
    void signalGameResult(const QString& user_pick_src, const QString& sign_src, const QString& comp_pick_src, const QString& result, const QString& color);

private:
    std::array<int, 3> LoadResults();
    std::vector<char> LoadChoices(const char* file_name);
    template<typename T>
    void ArrayToFile(const T& array, const char* file_name);
    void GameRound(char pick);
    char GetCpuChoice();
    char IntToChar(int input);
    int GetRoundResult(char choice, char cpu_choice);
    QString CharToSrc(char input);

    int computers_pattern_ = 0;
    std::array<int, 3> game_results_;
    std::vector<char> player_choices_;
    std::vector<char> cpu_choices_;
    QString user_pick_src_;
    QString comp_pick_src_;
    QString sign_src_;
    QString result_;
    QString color_;
};

#endif // RSP_ACTIONHANDLER_H
