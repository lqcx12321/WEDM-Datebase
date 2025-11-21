#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QVariantMap>

struct CandidateRow {
    int id;
    QString code;
    QVariantMap fields; // flexible container
};

#endif // TYPES_H