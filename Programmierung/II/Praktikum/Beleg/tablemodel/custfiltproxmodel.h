#ifndef _CUSTFILTPROXMODEL_H_
#define _CUSTFILTPROXMODEL_H_

#include <QSortFilterProxyModel>
#include <QStringList>
#include <QRegularExpression>


class CustomFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

    public:
        explicit CustomFilterProxyModel(QObject *parent)
            : QSortFilterProxyModel(parent) {}

        Q_INVOKABLE void setSearchString(const QString &filterText, const QString &delimiter);
        QList<QRegularExpression> getSearchTokens() const { return searchTokens; }

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    private:
        QList<QRegularExpression> searchTokens; // store search tokens as regex
};

#endif