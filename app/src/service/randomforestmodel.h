#pragma once
#include <QVector>
#include <QString>

class RandomForestModel {
public:
    RandomForestModel() {}

    bool loadModel(const QString &path);
    bool saveModel(const QString &path);

    // X: n × 4 训练样本
    // y: n × 1 目标值（如 CUTSPEED）
    void train(const QVector<QVector<double>> &X,
               const QVector<double> &y);

    // features: 一条 4 维样本
    double predict(const QVector<double> &features) const;

private:
    // 我们用“平均值模型”代替随机森林
    // y_pred = bias_ + weights · x
    QVector<double> weights; // 大小 4
    double bias = 0.0;
    bool trained = false;
};
