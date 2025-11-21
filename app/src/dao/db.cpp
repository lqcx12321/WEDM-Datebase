#include "db.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

static const char *DDL = R"(
CREATE TABLE IF NOT EXISTS CuttingProcessData (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    CODE TEXT,
    ON1 REAL,
    OFF1 REAL,
    IPM REAL,
    HP REAL,
    MA REAL,
    SV REAL,
    V REAL,
    SF REAL,
    WS REAL,
    WT REAL,
    W_MATERIAL TEXT,
    W_THICK REAL,
    D_DIAMETER REAL,
    D_MATERIAL TEXT,
    GAP REAL,
    CUTSPEED REAL,
    RA REAL,
    LAG REAL,
    remark TEXT
);
)";

DB::DB(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE", "wedm_conn");
    m_db.setDatabaseName(path);
}

DB::~DB() {
    QString connName = m_db.connectionName();
    m_db.close();
    QSqlDatabase::removeDatabase(connName);
}

bool DB::init(bool withDemo) {
    if (!m_db.open()) {
        qWarning() << "Open DB failed:" << m_db.lastError().text();
        return false;
    }
    QSqlQuery q(m_db);
    if (!q.exec(DDL)) {
        qWarning() << "DDL failed:" << q.lastError().text();
        return false;
    }
    // insert demo rows if empty
    if (!q.exec("SELECT COUNT(*) FROM CuttingProcessData")) return true;
    if (q.next()) {
        int c = q.value(0).toInt();
        if (c == 0 && withDemo) {
            QSqlQuery ins(m_db);
            ins.prepare("INSERT INTO CuttingProcessData (CODE,ON1,OFF1,IPM,HP,MA,SV,V,SF,WS,WT,W_MATERIAL,W_THICK,D_DIAMETER,D_MATERIAL,GAP,CUTSPEED,RA,LAG,remark) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
            ins.addBindValue("A01"); ins.addBindValue(1.2); ins.addBindValue(8.0); ins.addBindValue(8.0); ins.addBindValue(2); ins.addBindValue(0); ins.addBindValue(50); ins.addBindValue(80); ins.addBindValue(20); ins.addBindValue(3.0); ins.addBindValue(1200);
            ins.addBindValue("SKD11"); ins.addBindValue(20.0); ins.addBindValue(0.18); ins.addBindValue("黄铜"); ins.addBindValue(0.010); ins.addBindValue(120.0); ins.addBindValue(0.0008); ins.addBindValue(0.030); ins.addBindValue("demo");
            ins.exec();
            // add a couple more demo rows if needed (omitted for brevity)
        }
    }
    return true;
}

QList<CandidateRow> DB::queryAll() {
    QList<CandidateRow> out;
    QSqlQuery q(m_db);
    if (!q.exec("SELECT * FROM CuttingProcessData")) return out;
    while (q.next()) {
        CandidateRow r;
        r.id = q.value("id").toInt();
        r.code = q.value("CODE").toString();
        // store a few fields
        r.fields["CUTSPEED"] = q.value("CUTSPEED");
        r.fields["RA"] = q.value("RA");
        r.fields["LAG"] = q.value("LAG");
        r.fields["GAP"] = q.value("GAP");
        out.append(r);
    }
    return out;
}

bool DB::updateRemark(int id, const QString &remark) {
    QSqlQuery q(m_db);
    q.prepare("UPDATE CuttingProcessData SET remark = :r WHERE id = :i");
    q.bindValue(":r", remark);
    q.bindValue(":i", id);
    return q.exec();
}

QSqlDatabase& DB::connection() { return m_db; }