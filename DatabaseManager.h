#pragma once
#include <string>
#include "sqlite3.h"

class DatabaseManager {
private:
    sqlite3* db;

public:
    DatabaseManager();
    ~DatabaseManager();

    void saveScore(int score);
    std::string getTopScores();
};

