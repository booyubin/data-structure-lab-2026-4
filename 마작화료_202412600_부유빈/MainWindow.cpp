#include "MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    onReset();
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // 제목
    QLabel* titleLabel = new QLabel("🀄 마작 화료 경로 찾기");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // 패 개수 표시
    countLabel = new QLabel("손패: 0/14");
    countLabel->setStyleSheet("font-size: 14px; color: #666;");
    countLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(countLabel);
    
    // 손패 표시 영역 (스크롤 가능)
    QScrollArea* handScrollArea = new QScrollArea();
    handScrollArea->setWidgetResizable(true);
    handScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    handScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    handScrollArea->setMinimumHeight(120);
    handScrollArea->setMaximumHeight(120);
    
    QWidget* handContainer = new QWidget();
    handLayout = new QHBoxLayout(handContainer);
    handLayout->setAlignment(Qt::AlignLeft);
    handScrollArea->setWidget(handContainer);
    
    mainLayout->addWidget(handScrollArea);
    
    // 상태 표시
    statusLabel = new QLabel("손패를 설정하세요");
    statusLabel->setStyleSheet("font-size: 16px; margin: 10px;");
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);
    
    // 버튼들
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    findPathButton = new QPushButton("🔍 최단 경로 찾기");
    findPathButton->setStyleSheet("font-size: 16px; padding: 10px;");
    connect(findPathButton, &QPushButton::clicked, this, &MainWindow::onFindPath);
    
    resetButton = new QPushButton("🔄 샘플 손패");
    resetButton->setStyleSheet("font-size: 16px; padding: 10px;");
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
    
    clearButton = new QPushButton("🗑️ 전체 삭제");
    clearButton->setStyleSheet("font-size: 16px; padding: 10px;");
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClear);
    
    buttonLayout->addWidget(findPathButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(clearButton);
    mainLayout->addLayout(buttonLayout);
    
    // 경로 표시 영역
    pathLabel = new QLabel("");
    pathLabel->setStyleSheet("font-size: 14px; margin: 10px; padding: 10px; background: #f0f0f0;");
    pathLabel->setWordWrap(true);
    mainLayout->addWidget(pathLabel);
    
    // 패 선택기
    createTileSelector();
    
    setMinimumSize(1200, 700);
}

void MainWindow::createTileSelector() {
    selectorBox = new QGroupBox("패 선택 (클릭하여 손패에 추가)");
    selectorBox->setStyleSheet("font-size: 14px; font-weight: bold;");
    
    QVBoxLayout* selectorLayout = new QVBoxLayout(selectorBox);
    
    // 만수패
    QHBoxLayout* manLayout = new QHBoxLayout();
    QLabel* manLabel = new QLabel("만수:");
    manLabel->setStyleSheet("font-weight: bold; min-width: 50px;");
    manLayout->addWidget(manLabel);
    
    for (int i = 1; i <= 9; i++) {
        TileWidget* widget = new TileWidget(Tile(Tile::MAN, i));
        widget->setFixedSize(60, 80);
        connect(widget, &TileWidget::tileClicked, this, &MainWindow::onSelectorTileClicked);
        manLayout->addWidget(widget);
        selectorWidgets.push_back(widget);
    }
    manLayout->addStretch();
    selectorLayout->addLayout(manLayout);
    
    // 통수패
    QHBoxLayout* pinLayout = new QHBoxLayout();
    QLabel* pinLabel = new QLabel("통수:");
    pinLabel->setStyleSheet("font-weight: bold; min-width: 50px;");
    pinLayout->addWidget(pinLabel);
    
    for (int i = 1; i <= 9; i++) {
        TileWidget* widget = new TileWidget(Tile(Tile::PIN, i));
        widget->setFixedSize(60, 80);
        connect(widget, &TileWidget::tileClicked, this, &MainWindow::onSelectorTileClicked);
        pinLayout->addWidget(widget);
        selectorWidgets.push_back(widget);
    }
    pinLayout->addStretch();
    selectorLayout->addLayout(pinLayout);
    
    // 삭수패
    QHBoxLayout* souLayout = new QHBoxLayout();
    QLabel* souLabel = new QLabel("삭수:");
    souLabel->setStyleSheet("font-weight: bold; min-width: 50px;");
    souLayout->addWidget(souLabel);
    
    for (int i = 1; i <= 9; i++) {
        TileWidget* widget = new TileWidget(Tile(Tile::SOU, i));
        widget->setFixedSize(60, 80);
        connect(widget, &TileWidget::tileClicked, this, &MainWindow::onSelectorTileClicked);
        souLayout->addWidget(widget);
        selectorWidgets.push_back(widget);
    }
    souLayout->addStretch();
    selectorLayout->addLayout(souLayout);
    
    mainLayout->addWidget(selectorBox);
}

void MainWindow::updateHandDisplay() {
    // 기존 위젯 제거
    for (auto widget : handWidgets) {
        handLayout->removeWidget(widget);
        delete widget;
    }
    handWidgets.clear();
    
    // 새 위젯 생성
    for (const auto& tile : currentHand.getTiles()) {
        TileWidget* widget = new TileWidget(tile);
        widget->setToolTip("클릭하여 제거");
        connect(widget, &TileWidget::tileClicked, this, &MainWindow::onHandTileClicked);
        handLayout->addWidget(widget);
        handWidgets.push_back(widget);
    }
    
    updateStatus();
}

void MainWindow::updateStatus() {
    // 패 개수 표시
    int count = currentHand.size();
    countLabel->setText(QString("손패: %1/14").arg(count));
    
    if (count < 13) {
        countLabel->setStyleSheet("font-size: 14px; color: #ff6b6b;");
    } else if (count == 13) {
        countLabel->setStyleSheet("font-size: 14px; color: #ffa500;");
    } else if (count == 14) {
        countLabel->setStyleSheet("font-size: 14px; color: #51cf66;");
    } else {
        countLabel->setStyleSheet("font-size: 14px; color: #ff0000; font-weight: bold;");
    }
    
    // 상태 업데이트
    if (count != 14 && count != 13) {
        statusLabel->setText(QString("패를 %1장 더 추가하세요").arg(14 - count));
        statusLabel->setStyleSheet("font-size: 16px; color: #666;");
        return;
    }
    
    if (count == 14 && WinChecker::canWin(currentHand)) {
        statusLabel->setText("✅ 화료!");
        statusLabel->setStyleSheet("font-size: 18px; color: green; font-weight: bold;");
    } else if (count == 13 && WinChecker::isTenpai(currentHand)) {
        auto waiting = WinChecker::getWaitingTiles(currentHand);
        QString waitStr = "대기패: ";
        for (const auto& tile : waiting) {
            waitStr += QString::fromStdString(tile.toString()) + " ";
        }
        statusLabel->setText("🎯 텐파이! " + waitStr);
        statusLabel->setStyleSheet("font-size: 16px; color: orange; font-weight: bold;");
    } else {
        int shanten = WinChecker::getShanten(currentHand);
        statusLabel->setText(QString("향청패: %1").arg(shanten));
        statusLabel->setStyleSheet("font-size: 16px; color: blue;");
    }
}

void MainWindow::onHandTileClicked(Tile tile) {
    // 손패에서 제거
    currentHand.removeTile(tile);
    currentHand.sort();
    updateHandDisplay();
    pathLabel->setText("");
}

void MainWindow::onSelectorTileClicked(Tile tile) {
    // 14장 제한
    if (currentHand.size() >= 14) {
        QMessageBox::warning(this, "경고", "손패는 최대 14장까지만 가능합니다!");
        return;
    }
    
    // 손패에 추가
    currentHand.addTile(tile);
    currentHand.sort();
    updateHandDisplay();
    pathLabel->setText("");
}

void MainWindow::onFindPath() {
    int handSize = currentHand.size();
    
    // 13장 텐파이 처리
    if (handSize == 13) {
        if (WinChecker::isTenpai(currentHand)) {
            auto waiting = WinChecker::getWaitingTiles(currentHand);
            
            QString result = "🎯 텐파이 상태입니다!\n\n";
            result += "대기패 (" + QString::number(waiting.size()) + "종류):\n";
            
            for (const auto& tile : waiting) {
                result += "• " + QString::fromStdString(tile.toString()) + "\n";
            }
            
            result += "\n위 패 중 하나를 가져오면 화료입니다!";
            
            pathLabel->setText(result);
            pathLabel->setStyleSheet("font-size: 14px; margin: 10px; padding: 10px; background: #fff3cd;");
        } else {
            pathLabel->setText("❌ 텐파이가 아닙니다.\n\n패를 1장 더 추가하거나, 14장으로 만들어 경로를 찾아보세요.");
            pathLabel->setStyleSheet("font-size: 14px; margin: 10px; padding: 10px; background: #ffe0e0;");
        }
        return;
    }
    
    // 14장 아니면 경고
    if (handSize != 14) {
        QMessageBox::warning(this, "경고", "손패가 13장(텐파이) 또는 14장이어야 합니다!");
        return;
    }
    
    // 14장 화료 체크
    if (WinChecker::canWin(currentHand)) {
        QMessageBox::information(this, "알림", "이미 화료 상태입니다!");
        return;
    }
    
    // 버튼 비활성화
    findPathButton->setEnabled(false);
    statusLabel->setText("경로 탐색 중... (최대 2단계)");
    pathLabel->setText("탐색 중입니다. 잠시만 기다려주세요...");
    pathLabel->setStyleSheet("font-size: 14px; margin: 10px; padding: 10px; background: #fff3cd;");
    QApplication::processEvents();
    
    // 2단계까지만 탐색
    auto path = PathFinder::findPath(currentHand, 2);
    
    // 버튼 다시 활성화
    findPathButton->setEnabled(true);
    
    if (path.empty()) {
        pathLabel->setText("❌ 2단계 이내에 화료 경로를 찾을 수 없습니다.\n\n더 깊은 탐색이 필요하거나 화료가 불가능한 손패입니다.");
        pathLabel->setStyleSheet("font-size: 14px; margin: 10px; padding: 10px; background: #ffe0e0;");
    } else {
        displayPath(path);
    }
    
    updateStatus();
}

void MainWindow::displayPath(const std::vector<PathStep>& path) {
    QString result = QString("✅ %1단계 경로 발견!\n\n").arg(path.size());
    
    for (size_t i = 0; i < path.size(); i++) {
        result += QString("단계 %1: %2 버림 → %3 가져옴\n")
            .arg(i + 1)
            .arg(QString::fromStdString(path[i].discard.toString()))
            .arg(QString::fromStdString(path[i].draw.toString()));
    }
    
    pathLabel->setText(result);
    pathLabel->setStyleSheet("font-size: 14px; margin: 10px; padding: 10px; background: #e0ffe0;");
}

void MainWindow::onReset() {
    // 테스트용 손패 (1향청)
    currentHand.clear();
    currentHand.addTile(Tile(Tile::MAN, 1));
    currentHand.addTile(Tile(Tile::MAN, 2));
    currentHand.addTile(Tile(Tile::MAN, 4));
    currentHand.addTile(Tile(Tile::MAN, 5));
    currentHand.addTile(Tile(Tile::MAN, 6));
    currentHand.addTile(Tile(Tile::MAN, 7));
    currentHand.addTile(Tile(Tile::MAN, 8));
    currentHand.addTile(Tile(Tile::MAN, 9));
    currentHand.addTile(Tile(Tile::PIN, 1));
    currentHand.addTile(Tile(Tile::PIN, 1));
    currentHand.addTile(Tile(Tile::PIN, 1));
    currentHand.addTile(Tile(Tile::SOU, 5));
    currentHand.addTile(Tile(Tile::SOU, 5));
    currentHand.addTile(Tile(Tile::SOU, 7));
    
    currentHand.sort();
    updateHandDisplay();
    pathLabel->setText("");
}

void MainWindow::onClear() {
    currentHand.clear();
    updateHandDisplay();
    pathLabel->setText("");
}
