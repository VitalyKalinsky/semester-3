#include "WidgetContainer.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QDebug>
#include <cstring>
#include <QLabel>
#include <QSlider>
#include <QScrollBar>
#include <QSpinBox>

WidgetContainer::WidgetContainer(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Dynamic Widgets MetaObject Connector");
    this->setFixedSize(500, 600);
    auto *mainLayout = new QVBoxLayout(this);

    m_typeSelector = new QComboBox(this);
    m_typeSelector->addItem("QSlider", WidgetType::Slider);
    m_typeSelector->addItem("QScrollBar", WidgetType::ScrollBar);
    m_typeSelector->addItem("QSpinBox", WidgetType::SpinBox);
    m_typeSelector->addItem("QLabel", WidgetType::Label);
    mainLayout->addWidget(m_typeSelector);

    m_btnAdd = new QPushButton("Добавить виджет", this);
    mainLayout->addWidget(m_btnAdd);

    m_btnConnect = new QPushButton("Соединить все виджеты", this);
    mainLayout->addWidget(m_btnConnect);

    m_btnDebug = new QPushButton("Проверить соединения", this);
    mainLayout->addWidget(m_btnDebug);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto *containerWidget = new QWidget();

    m_widgetsLayout = new QVBoxLayout(containerWidget);
    m_widgetsLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(containerWidget);
    mainLayout->addWidget(scrollArea);

    connect(m_btnAdd, SIGNAL(clicked()), this, SLOT(addWidgetDynamically()));
    connect(m_btnConnect, SIGNAL(clicked()), this, SLOT(connectAllWidgets()));
    connect(m_btnDebug, SIGNAL(clicked()), this, SLOT(debugCheckConnections()));
}

void WidgetContainer::addWidgetDynamically()
{
    WidgetType selectedType = static_cast<WidgetType>(m_typeSelector->currentData().toInt());

    QWidget *newWidget = nullptr;

    switch (selectedType)
    {
    case WidgetType::Slider:
    {
        auto *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(0, 100);
        newWidget = slider;
        break;
    }
    case WidgetType::ScrollBar:
    {
        auto *scrollBar = new QScrollBar(Qt::Horizontal, this);
        scrollBar->setRange(0, 100);
        newWidget = scrollBar;
        break;
    }
    case WidgetType::SpinBox:
    {
        auto *spinBox = new QSpinBox(this);
        spinBox->setRange(0, 100);
        newWidget = spinBox;
        break;
    }
    case WidgetType::Label:
    {
        auto *label = new QLabel("0", this);
        newWidget = label;
        break;
    }
    default:
        break;
    }

    if (!newWidget)
        return;

    m_widgets.append(newWidget);
    m_widgetsLayout->addWidget(newWidget);

    qDebug() << "\tAdded:" << newWidget->metaObject()->className()
             << "\tTotal widgets:" << m_widgets.size();
}

void WidgetContainer::connectAllWidgets()
{
    qDebug() << "\n>>> Connecting widgets...";

    for (int i = 0; i < m_widgets.size(); ++i)
    {
        QWidget *sender = m_widgets[i];

        const char *senderClass = sender->metaObject()->className();

        bool canEmit = (std::strcmp(senderClass, "QSlider") == 0 ||
                        std::strcmp(senderClass, "QScrollBar") == 0 ||
                        std::strcmp(senderClass, "QSpinBox") == 0);

        if (!canEmit)
        {
            continue;
        }

        for (int j = 0; j < m_widgets.size(); ++j)
        {
            if (i == j)
                continue;

            QWidget *receiver = m_widgets[j];
            const char *receiverClass = receiver->metaObject()->className();

            const char *targetSlot = nullptr;
            if (std::strcmp(receiverClass, "QLabel") == 0)
            {
                targetSlot = SLOT(setNum(int));
            }
            else if (std::strcmp(receiverClass, "QSlider") == 0 ||
                     std::strcmp(receiverClass, "QScrollBar") == 0 ||
                     std::strcmp(receiverClass, "QSpinBox") == 0)
            {
                targetSlot = SLOT(setValue(int));
            }

            if (targetSlot)
            {
                bool connected = QObject::connect(sender, SIGNAL(valueChanged(int)), receiver, targetSlot, Qt::UniqueConnection);

                if (connected)
                {
                    qDebug() << " Connected:" << senderClass << "->" << receiverClass;
                }
                else
                {
                    qDebug() << " Duplicate:" << senderClass << "->" << receiverClass;
                }
            }
        }
    }
    qDebug() << ">>> Connection finished.\n";
}

void WidgetContainer::debugCheckConnections()
{
    qDebug() << "\n================= Debug connections =================";
    qDebug() << "N elements:" << m_widgets.size();

    for (int i = 0; i < m_widgets.size(); ++i)
    {
        QWidget *w = m_widgets[i];
        qDebug() << "\n--- Object #" << i + 1 << ":" << w->metaObject()->className() << "address:" << w << "---";

        w->dumpObjectInfo();
    }
    qDebug() << "==============================================================\n";
}