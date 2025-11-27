#include "recastdialog.h"
#include <QFormLayout>
#include <QPushButton>

RecastDialog::RecastDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("重熔层筛选设置");

    cb_enable = new QCheckBox("启用重熔层筛选");

    // ---- 加工等级选择 ----
    cb_grade = new QComboBox();
    cb_grade->addItems({"1级", "2级", "3级", "4级", "5级"});
    connect(cb_grade, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RecastDialog::onGradeChanged);

    // ---- 单独最大厚度限制（你原有功能） ----
    sp_limit = new QDoubleSpinBox();
    sp_limit->setDecimals(4);
    sp_limit->setRange(0, 1e3);
    sp_limit->setValue(0.01);

    // ---- 展示当前等级的标准参数（只读） ----
    lb_avg = new QLabel("-");
    lb_local = new QLabel("-");
    lb_crack_num = new QLabel("-");
    lb_crack_len = new QLabel("-");

    QPushButton *btn_ok = new QPushButton("确定");
    connect(btn_ok, &QPushButton::clicked, this, &RecastDialog::accept);

    // ---- 布局 ----
    QFormLayout *f = new QFormLayout(this);
    f->addRow(cb_enable);
    f->addRow("加工等级:", cb_grade);
    f->addRow("等级平均厚度(示例):", lb_avg);
    f->addRow("等级局部最大厚度:", lb_local);
    f->addRow("最大裂纹条数:", lb_crack_num);
    f->addRow("最大裂纹长度:", lb_crack_len);
    f->addRow("自定义最大厚度:", sp_limit);
    f->addRow(btn_ok);

    // 初次加载默认等级
    onGradeChanged(0);
}

void RecastDialog::onGradeChanged(int idx)
{
    // ---- 示例值，你之后按照你给我的标准表替换 ----
    struct Spec {
        double avg;
        double local;
        int crack_num;
        double crack_len;
    };

    static Spec table[5] = {
        {0.012, 0.025, 1, 0.025},   // 1级 —— 实际值你补
        {0.012, 0.025, 2, 0.035},   // 2级
        {0.035, 0.075, 4, 0.075},   // 3级
        {0.075, 0.150, 6, 0.150},   // 4级
        {0.100, 0.380, 10, 0.200}   // 5级
    };

    const Spec &s = table[idx];

    lb_avg->setText(QString::number(s.avg, 'f', 3));
    lb_local->setText(QString::number(s.local, 'f', 3));
    lb_crack_num->setText(QString::number(s.crack_num));
    lb_crack_len->setText(QString::number(s.crack_len, 'f', 3));
}
