#ifndef SELECTIONSERVICE_H
#define SELECTIONSERVICE_H

#include "../dao/db.h"
#include <QList>
#include <QString>
#include "../model/types.h"

class SelectionService {
public:
    SelectionService(DB *db);
    QList<CandidateRow> rankedCandidates(double w_speed, double w_ra, double w_lag, double w_gap, double target_gap);
private:
    DB *m_db;
};

#endif // SELECTIONSERVICE_H