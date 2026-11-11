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
    this->setWindowTitle("Соединитель штучек");
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

    m_btnRemove = new QPushButton("Удалить последний виджет", this);
    mainLayout->addWidget(m_btnRemove);

    m_removeSelector = new QComboBox(this);
    mainLayout->addWidget(m_removeSelector);

    m_btnRemoveSelected = new QPushButton("Удалить выбранный виджет", this);
    mainLayout->addWidget(m_btnRemoveSelected);

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
    connect(m_btnRemove, SIGNAL(clicked()), this, SLOT(removeLastWidget()));
    connect(m_btnRemoveSelected, SIGNAL(clicked()), this, SLOT(removeSelectedWidget()));
    connect(m_btnConnect, SIGNAL(clicked()), this, SLOT(connectAllWidgets()));
    connect(m_btnDebug, SIGNAL(clicked()), this, SLOT(debugCheckConnections()));
}

void WidgetContainer::addWidgetDynamically()
{
    WidgetType selectedType = static_cast<WidgetType>(m_typeSelector->currentData().toInt());

    QWidget *newWidget = nullptr;
    QString typeName;

    switch (selectedType)
    {
    case WidgetType::Slider:
    {
        auto *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(0, 100);
        newWidget = slider;
        typeName = "Slider";
        break;
    }
    case WidgetType::ScrollBar:
    {
        auto *scrollBar = new QScrollBar(Qt::Horizontal, this);
        scrollBar->setRange(0, 100);
        newWidget = scrollBar;
        typeName = "ScrollBar";
        break;
    }
    case WidgetType::SpinBox:
    {
        auto *spinBox = new QSpinBox(this);
        spinBox->setRange(0, 100);
        newWidget = spinBox;
        typeName = "SpinBox";
        break;
    }
    case WidgetType::Label:
    {
        auto *label = new QLabel("0", this);
        newWidget = label;
        typeName = "Label";
        break;
    }
    default:
        break;
    }

    if (!newWidget)
        return;

    QString name = QString("%1_%2").arg(typeName).arg(++m_widgetCounter);
    newWidget->setObjectName(name);

    auto *itemWidget = new QWidget(this);
    auto *itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(0, 0, 0, 0);
    itemLayout->addWidget(new QLabel(name + ":", itemWidget));
    itemLayout->addWidget(newWidget);

    m_widgets.append(newWidget);
    m_widgetsLayout->addWidget(itemWidget);
    m_removeSelector->addItem(name, QVariant::fromValue(static_cast<void *>(newWidget)));

    qDebug() << "\tAdded:" << newWidget->metaObject()->className()
             << "Name:" << name
             << "\tTotal widgets:" << m_widgets.size();
}

void WidgetContainer::removeLastWidget()
{
    if (m_widgets.isEmpty())
        return;

    QWidget *w = m_widgets.takeLast();
    int idx = m_removeSelector->findData(QVariant::fromValue(static_cast<void *>(w)));
    if (idx != -1)
        m_removeSelector->removeItem(idx);

    delete w->parentWidget();
}

void WidgetContainer::removeSelectedWidget()
{
    if (m_removeSelector->currentIndex() == -1)
        return;

    auto *w = static_cast<QWidget *>(m_removeSelector->currentData().value<void *>());
    m_removeSelector->removeItem(m_removeSelector->currentIndex());
    m_widgets.removeOne(w);

    if (w)
        delete w->parentWidget();
}

void WidgetContainer::connectAllWidgets()
{
    qDebug() << "\n>>> Connecting widgets...";

    int maxValue = 0;
    for (QWidget *w : m_widgets)
    {
        int val = qobject_cast<QLabel *>(w) ? qobject_cast<QLabel *>(w)->text().toInt()
                                            : w->property("value").toInt();
        if (val > maxValue)
            maxValue = val;
    }

    for (QWidget *w : m_widgets)
    {
        if (auto *lbl = qobject_cast<QLabel *>(w))
            lbl->setNum(maxValue);
        else
            w->setProperty("value", maxValue);
    }

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
        qDebug() << "\n--- Object #" << i + 1 << ":" << w->metaObject()->className()
                 << "name:" << w->objectName() << "address:" << w << "---";

        w->dumpObjectInfo();
    }
    qDebug() << "==============================================================\n";
}
