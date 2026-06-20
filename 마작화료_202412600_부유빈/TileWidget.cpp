#include "TileWidget.h"
#include <QFont>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QPainter>
#include <QMouseEvent>
#include <QEnterEvent>

/**
 * @brief 생성자
 */
TileWidget::TileWidget(const Tile& t, QWidget* parent)
    : QWidget(parent), tile(t), selected(false), enabled(true), hovered(false) {
    
    setFixedSize(70, 90);
    setMouseTracking(true);
}

void TileWidget::setTile(const Tile& t) {
    tile = t;
    update();
}

void TileWidget::setSelected(bool s) {
    selected = s;
    update();
}

void TileWidget::setEnabled(bool e) {
    enabled = e;
    update();
}

/**
 * @brief 페인트 이벤트 - 메인 그리기 함수
 */
void TileWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    
    QRect tileRect = rect().adjusted(3, 3, -3, -3);
    
    // 배경 그리기
    drawTileBackground(painter, tileRect);
    
    // 패 내용 그리기 영역
    QRect contentRect = tileRect.adjusted(8, 10, -8, -10);
    
    // 패 종류에 따라 그리기
    switch (tile.getType()) {
        case Tile::MAN:
            drawManTile(painter, contentRect);
            break;
        case Tile::PIN:
            drawPinTile(painter, contentRect);
            break;
        case Tile::SOU:
            drawSouTile(painter, contentRect);
            break;
    }
}

/**
 * @brief 패 배경 그리기 (입체감 있게)
 */
void TileWidget::drawTileBackground(QPainter& painter, const QRect& rect) {
    // 상태에 따른 색상
    QColor baseColor = QColor(250, 248, 240);  // 아이보리색
    QColor borderColor = QColor(80, 80, 80);
    int borderWidth = 2;
    
    if (!enabled) {
        baseColor = QColor(200, 200, 200);
    } else if (selected) {
        baseColor = QColor(255, 250, 205);  // 연한 노란색
        borderColor = QColor(255, 140, 0);  // 주황색
        borderWidth = 3;
    } else if (hovered) {
        baseColor = QColor(240, 248, 255);  // 연한 파란색
        borderColor = QColor(100, 149, 237);
    }
    
    // 그라데이션 배경 (입체감)
    QLinearGradient bgGradient(rect.topLeft(), rect.bottomRight());
    bgGradient.setColorAt(0, baseColor.lighter(105));
    bgGradient.setColorAt(0.5, baseColor);
    bgGradient.setColorAt(1, baseColor.darker(105));
    
    painter.setBrush(bgGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 5, 5);
    
    // 그림자 효과
    if (!selected) {
        painter.setPen(QPen(QColor(0, 0, 0, 30), 1));
        painter.drawRoundedRect(rect.adjusted(1, 1, 1, 1), 5, 5);
    }
    
    // 테두리
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(borderColor, borderWidth));
    painter.drawRoundedRect(rect, 5, 5);
    
    // 안쪽 테두리 (디테일)
    painter.setPen(QPen(borderColor.lighter(150), 1));
    painter.drawRoundedRect(rect.adjusted(borderWidth, borderWidth, 
                                         -borderWidth, -borderWidth), 4, 4);
}

/**
 * @brief 만수패 그리기 (한자 + 萬)
 */
void TileWidget::drawManTile(QPainter& painter, const QRect& rect) {
    // 한자 숫자
    static const QString manChars[] = {
        "一", "二", "三", "四", "五", "六", "七", "八", "九"
    };
    
    int num = tile.getNumber();
    if (num < 1 || num > 9) return;
    
    // 상단: 큰 한자 숫자 (빨간색)
    QRect topRect = rect;
    topRect.setHeight(rect.height() * 0.7);
    
    QFont numberFont("Arial", 32, QFont::Bold);
    painter.setFont(numberFont);
    
    // 그림자 효과
    painter.setPen(QColor(100, 0, 0, 80));
    painter.drawText(topRect.adjusted(2, 2, 2, 2), 
                     Qt::AlignCenter, manChars[num - 1]);
    
    // 메인 텍스트
    painter.setPen(QColor(200, 0, 0));  // 진한 빨간색
    painter.drawText(topRect, Qt::AlignCenter, manChars[num - 1]);
    
    // 하단: "萬" 글자 (검은색)
    QRect bottomRect = rect;
    bottomRect.setTop(rect.bottom() - rect.height() * 0.3);
    
    QFont wanFont("Arial", 14, QFont::Bold);
    painter.setFont(wanFont);
    painter.setPen(Qt::black);
    painter.drawText(bottomRect, Qt::AlignCenter, "萬");
}

/**
 * @brief 통수패 그리기 (원 패턴)
 */
void TileWidget::drawPinTile(QPainter& painter, const QRect& rect) {
    int num = tile.getNumber();
    if (num < 1 || num > 9) return;
    
    // 원 배치 패턴 (x, y는 0.0~1.0 비율)
    struct Circle {
        double x, y;
        bool isRed;  // 5통의 중앙 원은 빨간색
    };
    
    std::vector<Circle> pattern;
    
    switch (num) {
        case 1:
            pattern = {{0.5, 0.5, false}};
            break;
        case 2:
            pattern = {{0.35, 0.35, false}, {0.65, 0.65, false}};
            break;
        case 3:
            pattern = {{0.35, 0.35, false}, {0.5, 0.5, false}, {0.65, 0.65, false}};
            break;
        case 4:
            pattern = {{0.35, 0.35, false}, {0.65, 0.35, false}, 
                      {0.35, 0.65, false}, {0.65, 0.65, false}};
            break;
        case 5:
            pattern = {{0.35, 0.35, false}, {0.65, 0.35, false}, 
                      {0.5, 0.5, true},    // 중앙은 빨간색
                      {0.35, 0.65, false}, {0.65, 0.65, false}};
            break;
        case 6:
            pattern = {{0.35, 0.25, false}, {0.65, 0.25, false},
                      {0.35, 0.5, false},  {0.65, 0.5, false},
                      {0.35, 0.75, false}, {0.65, 0.75, false}};
            break;
        case 7:
            pattern = {{0.35, 0.25, false}, {0.65, 0.25, false},
                      {0.35, 0.5, false},  {0.5, 0.5, false}, {0.65, 0.5, false},
                      {0.35, 0.75, false}, {0.65, 0.75, false}};
            break;
        case 8:
            pattern = {{0.3, 0.2, false}, {0.5, 0.2, false}, {0.7, 0.2, false},
                      {0.3, 0.5, false},                     {0.7, 0.5, false},
                      {0.3, 0.8, false}, {0.5, 0.8, false}, {0.7, 0.8, false}};
            break;
        case 9:
            pattern = {{0.3, 0.2, false}, {0.5, 0.2, false}, {0.7, 0.2, false},
                      {0.3, 0.5, false}, {0.5, 0.5, false}, {0.7, 0.5, false},
                      {0.3, 0.8, false}, {0.5, 0.8, false}, {0.7, 0.8, false}};
            break;
    }
    
    // 원 크기 결정
    int circleRadius = (num <= 4) ? 10 : (num <= 6) ? 8 : 7;
    
    // 각 원 그리기
    for (const auto& circle : pattern) {
        int cx = rect.left() + rect.width() * circle.x;
        int cy = rect.top() + rect.height() * circle.y;
        
        QColor color = circle.isRed ? QColor(200, 0, 0) : QColor(0, 100, 200);
        
        // 그라데이션으로 입체감
        QRadialGradient gradient(cx, cy, circleRadius);
        gradient.setColorAt(0, color.lighter(130));
        gradient.setColorAt(0.7, color);
        gradient.setColorAt(1, color.darker(120));
        
        painter.setBrush(gradient);
        painter.setPen(QPen(color.darker(150), 2));
        painter.drawEllipse(QPoint(cx, cy), circleRadius, circleRadius);
        
        // 하이라이트 (광택 효과)
        painter.setBrush(QColor(255, 255, 255, 100));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(cx - circleRadius/3, cy - circleRadius/3), 
                           circleRadius/3, circleRadius/3);
    }
}

/**
 * @brief 삭수패 그리기 (대나무)
 */
void TileWidget::drawSouTile(QPainter& painter, const QRect& rect) {
    int num = tile.getNumber();
    if (num < 1 || num > 9) return;
    
    // 대나무 막대 배치
    struct Bamboo {
        double x, y, height;
    };
    
    std::vector<Bamboo> pattern;
    
    switch (num) {
        case 1:
            pattern = {{0.5, 0.2, 0.7}};
            break;
        case 2:
            pattern = {{0.4, 0.2, 0.7}, {0.6, 0.2, 0.7}};
            break;
        case 3:
            pattern = {{0.35, 0.2, 0.65}, {0.5, 0.2, 0.65}, {0.65, 0.2, 0.65}};
            break;
        case 4:
            pattern = {{0.35, 0.15, 0.55}, {0.65, 0.15, 0.55},
                      {0.35, 0.55, 0.55}, {0.65, 0.55, 0.55}};
            break;
        case 5:
            pattern = {{0.3, 0.15, 0.5}, {0.5, 0.15, 0.5}, {0.7, 0.15, 0.5},
                      {0.4, 0.55, 0.5}, {0.6, 0.55, 0.5}};
            break;
        case 6:
            pattern = {{0.3, 0.15, 0.45}, {0.5, 0.15, 0.45}, {0.7, 0.15, 0.45},
                      {0.3, 0.5, 0.45},  {0.5, 0.5, 0.45},  {0.7, 0.5, 0.45}};
            break;
        case 7:
            pattern = {{0.3, 0.1, 0.4}, {0.5, 0.1, 0.4}, {0.7, 0.1, 0.4},
                      {0.3, 0.45, 0.4}, {0.5, 0.45, 0.4}, {0.7, 0.45, 0.4},
                      {0.5, 0.75, 0.4}};
            break;
        case 8:
            pattern = {{0.3, 0.1, 0.35}, {0.5, 0.1, 0.35}, {0.7, 0.1, 0.35},
                      {0.3, 0.4, 0.35}, {0.5, 0.4, 0.35}, {0.7, 0.4, 0.35},
                      {0.4, 0.7, 0.35}, {0.6, 0.7, 0.35}};
            break;
        case 9:
            pattern = {{0.3, 0.1, 0.3}, {0.5, 0.1, 0.3}, {0.7, 0.1, 0.3},
                      {0.3, 0.4, 0.3}, {0.5, 0.4, 0.3}, {0.7, 0.4, 0.3},
                      {0.3, 0.7, 0.3}, {0.5, 0.7, 0.3}, {0.7, 0.7, 0.3}};
            break;
    }
    
    // 각 대나무 그리기
    for (const auto& bamboo : pattern) {
        int x = rect.left() + rect.width() * bamboo.x;
        int y = rect.top() + rect.height() * bamboo.y;
        int h = rect.height() * bamboo.height;
        
        QRect bambooRect(x - 4, y, 8, h);
        
        // 대나무 그라데이션 (입체감)
        QLinearGradient gradient(bambooRect.topLeft(), bambooRect.topRight());
        gradient.setColorAt(0, QColor(40, 120, 40));
        gradient.setColorAt(0.3, QColor(60, 160, 60));
        gradient.setColorAt(0.7, QColor(60, 160, 60));
        gradient.setColorAt(1, QColor(40, 120, 40));
        
        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(30, 90, 30), 1));
        painter.drawRoundedRect(bambooRect, 3, 3);
        
        // 대나무 마디 (3개)
        painter.setPen(QPen(QColor(30, 90, 30), 2));
        int node1 = y + h * 0.25;
        int node2 = y + h * 0.5;
        int node3 = y + h * 0.75;
        
        painter.drawLine(x - 4, node1, x + 4, node1);
        painter.drawLine(x - 4, node2, x + 4, node2);
        painter.drawLine(x - 4, node3, x + 4, node3);
        
        // 하이라이트 (광택)
        painter.setPen(QPen(QColor(100, 200, 100, 100), 2));
        painter.drawLine(x - 2, y + 2, x - 2, y + h - 2);
    }
}

/**
 * @brief 마우스 클릭 이벤트
 */
void TileWidget::mousePressEvent(QMouseEvent* event) {
    if (enabled && event->button() == Qt::LeftButton) {
        selected = !selected;
        update();
        emit tileClicked(tile);
    }
}

/**
 * @brief 마우스 호버 시작
 */
void TileWidget::enterEvent(QEnterEvent* event) {
    Q_UNUSED(event);
    if (enabled) {
        hovered = true;
        update();
        setCursor(Qt::PointingHandCursor);
    }
}

/**
 * @brief 마우스 호버 종료
 */
void TileWidget::leaveEvent(QEvent* event) {
    Q_UNUSED(event);
    hovered = false;
    update();
    setCursor(Qt::ArrowCursor);
}
