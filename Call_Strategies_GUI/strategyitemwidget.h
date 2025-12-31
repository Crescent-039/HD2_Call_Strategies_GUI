#ifndef STRATEGYITEMWIDGET_H
#define STRATEGYITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include "strategyitem.h"


namespace Ui { class StrategyItemWidget; }


class StrategyItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StrategyItemWidget(QWidget *parent = nullptr);
    // 在widget里让已经选择的战备可见
    void setStrategy(const StrategyItem& strategy);

private:
    Ui::StrategyItemWidget *ui;

signals:

};

#endif // STRATEGYITEMWIDGET_H
