#include "DatabaseManager.h"
#include <iostream>
#include <ctime>


DatabaseManager::DatabaseManager() : db(nullptr) {
    int rc = sqlite3_open("scores.db", &db);
    if (rc) {
        std::cerr << "Не удалось открыть сейф: " << sqlite3_errmsg(db) << std::endl;
        return;
    }


    const char* sql = "CREATE TABLE IF NOT EXISTS high_scores ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "score INTEGER NOT NULL,"
                      "date TEXT NOT NULL);";
    
    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}


DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}


void DatabaseManager::saveScore(int score) {
    if (score <= 0) return;


    std::time_t t = std::time(nullptr);
    char dateStr[100];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M", std::localtime(&t));

    std::string sql = "INSERT INTO high_scores (score, date) VALUES (" + std::to_string(score) + ", '" + std::string(dateStr) + "');";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка записи рекорда: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

std::string DatabaseManager::getTopScores() {
    std::string result = "";
    

    const char* sql = "SELECT score, date FROM high_scores ORDER BY score DESC LIMIT 5;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK) {
        int rank = 1;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int savedScore = sqlite3_column_int(stmt, 0);
            const unsigned char* date = sqlite3_column_text(stmt, 1);

            result += std::to_string(rank) + ".  Score: " + std::to_string(savedScore) + "   (" + reinterpret_cast<const char*>(date) + ")\n\n";
            rank++;
        }
    }
    sqlite3_finalize(stmt);

    if (result.empty()) {
        result = "Рекордов пока нет. Будь первым!";
    }
    return result;
}