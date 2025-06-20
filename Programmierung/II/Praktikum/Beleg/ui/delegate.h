#include <QSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>

#include "infopanel.h"

class InfoPanelDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    InfoPanelDelegate(InfoPanel* panel, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), infoPanelPtr(panel) {}

    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;


    void setEditorData(QWidget* editor, const QModelIndex& index) const override;


    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    private:
        InfoPanel* infoPanelPtr; // pointer to the InfoPanel instance
};