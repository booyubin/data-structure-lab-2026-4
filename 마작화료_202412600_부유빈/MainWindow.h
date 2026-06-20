#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <vector>
#include "TileWidget.h"
#include "Hand.h"
#include "PathFinder.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Hand currentHand;
    std::vector<TileWidget*> handWidgets;
    std::vector<TileWidget*> selectorWidgets;  // ← 추가
    
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QHBoxLayout* handLayout;
    QLabel* statusLabel;
    QLabel* pathLabel;
    QLabel* countLabel;  // ← 추가 (패 개수 표시)
    QPushButton* findPathButton;
    QPushButton* resetButton;
    QPushButton* clearButton;  // ← 추가
    QGroupBox* selectorBox;  // ← 추가

public:
    explicit MainWindow(QWidget *parent = nullptr);
    
private slots:
    void onFindPath();
    void onReset();
    void onClear();  // ← 추가
    void onHandTileClicked(Tile tile);  // ← 이름 변경
    void onSelectorTileClicked(Tile tile);  // ← 추가
    
private:
    void setupUI();
    void createTileSelector();  // ← 추가
    void updateHandDisplay();
    void updateStatus();  // ← 추가
    void displayPath(const std::vector<PathStep>& path);
};

#endif // MAINWINDOW_H
