#ifndef RECASTDIALOG_H
#define RECASTDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>

class RecastDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecastDialog(QWidget *parent = nullptr);

    bool enableRecast() const { return cb_enable->isChecked(); }
    double limitThickness() const { return sp_limit->value(); }
    int selectedGrade() const { return cb_grade->currentIndex() + 1; }

private slots:
    void onGradeChanged(int idx);

private:
    QCheckBox *cb_enable;
    QDoubleSpinBox *sp_limit;
    QComboBox *cb_grade;

    QLabel *lb_avg;
    QLabel *lb_local;
    QLabel *lb_crack_num;
    QLabel *lb_crack_len;
};

#endif // RECASTDIALOG_H
