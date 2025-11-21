#ifndef DB_H
#define DB_H

#include <QString>
#include <QSqlDatabase>
#include <QList>
#include "../model/types.h"

class DB {
public:
    DB(const QString &path);
    ~DB();
    bool init(bool withDemo=true);
    QList<CandidateRow> queryAll();
    bool updateRemark(int id, const QString &remark);
    QSqlDatabase& connection();
private:
    QSqlDatabase m_db;
};

#endif // DB_H
