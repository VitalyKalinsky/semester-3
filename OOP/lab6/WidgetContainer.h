#pragma once

#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

class WidgetContainer : public QWidget
{
    Q_OBJECT

public:
    enum WidgetType
    {
        Slider,
        ScrollBar,
        SpinBox,
        Label
    };
    explicit WidgetContainer(QWidget *parent = nullptr);
    ~WidgetContainer() override = default;

public slots:
    void addWidgetDynamically();
    void removeLastWidget();
    void removeSelectedWidget();
    void connectAllWidgets();
    void debugCheckConnections();

private:
    QList<QWidget *> m_widgets;

    QComboBox *m_typeSelector;
    QPushButton *m_btnAdd;
    QPushButton *m_btnRemove;
    QPushButton *m_btnConnect;
    QPushButton *m_btnDebug;
    QVBoxLayout *m_widgetsLayout;

    QComboBox *m_removeSelector;
    QPushButton *m_btnRemoveSelected;
    int m_widgetCounter = 0;
};
