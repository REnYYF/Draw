#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QScrollArea>
#include <QUndoStack>
#include <QUndoView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QToolButton>
#include <QComboBox>
#include <QFileDialog> 
#include "graphicsproperty.h"
#include "drawview.h"
#include "drawscene.h"
#include "commands.h"

#ifdef Q_CC_MSVC
#pragma execution_character_set("utf-8")//解决中文在msvc编译器下的乱码问题
#endif


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void model(int index);
	void itemsChange(QGraphicsItem *item);

	//undo
	//void itemLineSize(QGraphicsScene *scene, const qreal oldSize, const qreal nerSize);
public slots:
    //file
    void newFile();
    void open();
    void save();

    //edit
    void on_copy();
    void on_paste();
    void on_cut();
    void deleteItem();

    //color
    void fillColor();
    void lineColor();
    void lineSize(QAction *action);

    //draw
    void addShape();

    //select
    void select1();
    void select2();
    void select3();

	//属性
	void onItemsChange(QGraphicsItem *item);
	void onValuesChange();
	
	//undo redo
	void itemAdded(QGraphicsItem *item);
	void itemRotate(QGraphicsItem *item, const qreal oldAngle);
	void itemLineSize(QGraphicsScene *scene, const qreal newSize);
	void itemLineColor(QGraphicsScene *scene, const QColor newColor);
	void itemBrushColor(QGraphicsScene *scene, const QColor newColor);
	void itemResize(QGraphicsItem *item, const QRectF rect);
	void itemMoved(QList<QGraphicsItem *> item);
private:
    //file
    void createActions();
    void createMenus();
    void createToolbars();


    //图形选择区 编辑区 属性区
    QScrollArea *createSelectArea(QWidget* widget);
    void createFunctionalArea();

	QMenu *createLineSizeMenu();

	void relationTo() const;
private:
    //图形选择区
    QScrollArea *m_pGraphicsSelect;

    //属性区
    GraphicsProperty *m_pGraPro;

    //编辑区
    DrawView *m_pEditView;
    DrawScene *m_scene;

    //undo redo
    QUndoStack *m_undoStack;
	QUndoView *m_undoView;

    //tool
    QToolBar *m_pEditToolBar;
    QToolBar *m_pFileToolBar;
    QToolBar *m_pColorToolBar;
    QToolBar *m_pDrawToolBar;

    // file action
    QAction *m_pNewAct;
    QAction *m_pOpenAct;
    QAction *m_pSaveAct;
    QAction *m_pExitAct;

    // edit action
    QAction  *m_pDeleteAct;
    QAction  *m_pUndoAct;
    QAction  *m_pRedoAct;
    QAction  *m_pCopyAct;
    QAction  *m_pPasteAct;
    QAction  *m_pCutAct;

    //color action
    QAction *m_pFillColor;
    QAction *m_pLineColor;
	QToolButton *m_pLineSize;

    //draw action
    QAction  *m_pLineAct;
    QAction  *m_pRectAct;
    QAction  *m_pEllipseAct;
	QAction  *m_pRotateAct;

	//line size menu
	QMenu    *m_menu;

};

#endif // MAINWINDOW_H
