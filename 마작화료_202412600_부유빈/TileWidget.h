#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QEnterEvent>
#include "Tile.h"

class TileWidget : public QWidget {
    Q_OBJECT

private:
    Tile tile;
    bool selected;
    bool enabled;
    bool hovered;

public:
    explicit TileWidget(const Tile& t, QWidget* parent = nullptr);

    Tile getTile() const { return tile; }
    void setTile(const Tile& t);
    bool isSelected() const { return selected; }
    void setSelected(bool s);
    void setEnabled(bool e);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

signals:
    void tileClicked(Tile tile);

private:
    void drawTileBackground(QPainter& painter, const QRect& rect);
    void drawManTile(QPainter& painter, const QRect& rect);
    void drawPinTile(QPainter& painter, const QRect& rect);
    void drawSouTile(QPainter& painter, const QRect& rect);
};

#endif
