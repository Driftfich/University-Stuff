/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the CustomFilterProxyModel class, which is a custom filter proxy model building on top of the QSortFilterProxyModel.
Purpose: Custom filter logic for the table view. The filter string is split into tokens by a delimiter.
Each row is accepted if it contains all of the tokens in any columns. This allows flexible multiple column searches.
*/

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

        // set the search string and delimiter
        Q_INVOKABLE void setSearchString(const QString &filterText, const QString &delimiter);

        // get the search tokens
        QList<QRegularExpression> getSearchTokens() const { return searchTokens; }

    protected:
        // override the filterAcceptsRow method to implement the custom filter logic
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    private:
        QList<QRegularExpression> searchTokens; // store search tokens as regex
};

#endif