#ifndef RECASTDIALOG_H
#define RECASTDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>

class RecastDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RecastDialog(QWidget *parent = nullptr);

private slots:
    void onGradeChanged(int idx);

private:
    QCheckBox *cb_enable;
    QComboBox *cb_grade;
    QDoubleSpinBox *sp_limit;
    QLabel *lb_avg;
    QLabel *lb_local;
    QLabel *lb_crack_num;
    QLabel *lb_crack_len;

    struct Spec {
        double avg;
        double local;
        int crack_num;
        double crack_len;

        bool avg_unlimited = false;       // true = 任意
        bool local_unlimited = false;     // true = 任意
        bool crack_num_unlimited = false; // true = 任意
        bool crack_len_none = false;      // true = 无
    };
};

#endif // RECASTDIALOG_H
