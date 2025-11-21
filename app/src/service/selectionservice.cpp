#include "selectionservice.h"
#include <algorithm>

SelectionService::SelectionService(DB *db): m_db(db) {}

QList<CandidateRow> SelectionService::rankedCandidates(double w_speed, double w_ra, double w_lag, double w_gap, double target_gap) {
    auto rows = m_db->queryAll();
    // simple scoring example using fields stored
    struct ScoreItem { CandidateRow r; double score; };
    QVector<ScoreItem> scores;

    // 归一化处理
    double sp_lo=1e9, sp_hi=0, ra_lo=1e9, ra_hi=0, lg_lo=1e9, lg_hi=0;
    for (auto &r: rows) {
        double s = r.fields.value("CUTSPEED", 0.0).toDouble();
        double ra = r.fields.value("RA", 0.0).toDouble();
        double lg = r.fields.value("LAG", 0.0).toDouble();
        sp_lo = std::min(sp_lo, s); sp_hi = std::max(sp_hi, s);
        ra_lo = std::min(ra_lo, ra); ra_hi = std::max(ra_hi, ra);
        lg_lo = std::min(lg_lo, lg); lg_hi = std::max(lg_hi, lg);
    }


    for (auto &r: rows) {
        double s = r.fields.value("CUTSPEED", 0.0).toDouble();
        double ra = r.fields.value("RA", 0.0).toDouble();
        double lg = r.fields.value("LAG", 0.0).toDouble();
        auto norm_pos = [](double x,double lo,double hi){ if (hi==lo) return 1.0; return (x-lo)/(hi-lo); };
        auto norm_neg = [](double x,double lo,double hi){ if (hi==lo) return 1.0; return 1.0-(x-lo)/(hi-lo); };
        
        // 计算最终评分
        double sc = 0.0;
        sc += norm_pos(s, sp_lo, sp_hi) * w_speed;
        sc += norm_neg(ra, ra_lo, ra_hi) * w_ra;
        sc += norm_neg(lg, lg_lo, lg_hi) * w_lag;
        if (target_gap>0) {
            double gap = r.fields.value("GAP", 0.0).toDouble();
            double rel = std::abs(gap - target_gap) / std::max(target_gap, 1e-9);
            sc += std::max(0.0, 1.0 - rel) * w_gap;
        }
        scores.append({r, sc});
    }
    std::sort(scores.begin(), scores.end(), [](const ScoreItem&a,const ScoreItem&b){ return a.score>b.score;});
    QList<CandidateRow> out;
    for (auto &it: scores) out.append(it.r);
    return out;
}
