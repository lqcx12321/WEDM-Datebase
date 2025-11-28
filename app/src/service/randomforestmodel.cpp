#include "randomforestmodel.h"
#include <QFile>
#include <QTextStream>

// --------------------------------------------------------
// 训练：简单平均拟合
// --------------------------------------------------------
void RandomForestModel::train(const QVector<QVector<double>> &X,
                              const QVector<double> &y)
{
    int n = X.size();
    if (n == 0) return;

    int dim = X[0].size();
    weights = QVector<double>(dim, 0.0);

    // 简单：每个特征权重 = 该特征平均值 / y 平均值
    QVector<double> meanX(dim, 0.0);
    double meanY = 0.0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < dim; ++j)
            meanX[j] += X[i][j];
        meanY += y[i];
    }

    for (int j = 0; j < dim; ++j)
        meanX[j] /= n;
    meanY /= n;

    for (int j = 0; j < dim; ++j)
        weights[j] = meanY == 0 ? 0.0 : (meanX[j] / meanY);

    bias = meanY;
    trained = true;
}

// --------------------------------------------------------
// 预测：简单线性组合
// --------------------------------------------------------
double RandomForestModel::predict(const QVector<double> &features) const
{
    if (!trained) return 0.0;

    double sum = bias;
    for (int i = 0; i < weights.size() && i < features.size(); ++i)
        sum += weights[i] * features[i];

    return sum;
}

// --------------------------------------------------------
// 保存模型到文本
// --------------------------------------------------------
bool RandomForestModel::saveModel(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&f);
    out << bias << "\n";
    out << weights.size() << "\n";
    for (double w : weights)
        out << w << "\n";

    f.close();
    return true;
}

// --------------------------------------------------------
// 读取模型
// --------------------------------------------------------
bool RandomForestModel::loadModel(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&f);
    in >> bias;

    int dim = 0;
    in >> dim;

    weights = QVector<double>(dim, 0.0);
    for (int i = 0; i < dim; ++i)
        in >> weights[i];

    trained = true;
    f.close();
    return true;
}
