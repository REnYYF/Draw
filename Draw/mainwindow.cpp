#include "mainwindow.h"
#include <QColorDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_undoStack = new QUndoStack(this);
    m_undoView = new QUndoView(m_undoStack);
    m_undoView->setWindowTitle(tr("Command List"));
    createActions();
    createMenus();
    createToolbars();
    createFunctionalArea();
    relationTo();
    resize(1200, 800);
}

MainWindow::~MainWindow()
{

}

void MainWindow::newFile()
{
    QList<QGraphicsItem*> itemTotal = m_scene->items();
    for each(auto var in itemTotal)
    {
        m_scene->removeItem(var);
    }
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                       "../../",
                       tr("Images (*.xml *.json)"));
    QString strSel = fileName.split(".").last();
    if (strSel == "xml")
    {
        m_scene->openFileFromXml(fileName);
    }
    if (strSel == "json")
    {
        m_scene->openFileFromJson(fileName);
    }
    qDebug() << fileName << endl;
}

void MainWindow::save()
{
    QList<QGraphicsItem*> itemTotal = m_scene->items();
    QList<QGraphicsItem*> items;
    for each(auto var in itemTotal)
    {
        if (var->type() != SIZE_HANDLE_RECT)
        {
            items.push_back(var);
        }
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                       "../../draw.xml",
                       tr("Images (*.xml *.json)"));
    QString strSel = fileName.split(".").last();
    if (strSel == "xml")
    {
        m_scene->saveFileToXml(fileName, items);
    }
    else if (strSel == "json")
    {
        m_scene->saveFileToJson(fileName, items);
    }
    else
    {
        qDebug() << "error,File type is not correct." << endl;
    }
    qDebug() << fileName << endl;
}

void MainWindow::on_copy()
{
    qDebug() << "on_copy()" << endl;
    m_scene->copyItem();
}

void MainWindow::on_paste()
{
    m_scene->pasteItem();
}

void MainWindow::on_cut()
{
    m_scene->shearItem();
    deleteItem();
}

void MainWindow::deleteItem()
{
    qDebug() << "void MainWindow::deleteItem()" << endl;

    if (m_scene->selectedItems().isEmpty())
        return;

    QUndoCommand* deleteCommand = new RemoveShapeCommand(m_scene);
    m_undoStack->push(deleteCommand);

}

void MainWindow::fillColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, tr("设置填充颜色"));
    if (color.isValid())
    {
        QList<QGraphicsItem*> itemList = m_scene->selectedItems();
        if (itemList.isEmpty())
            return;
        itemBrushColor(m_scene, color);
        emit itemsChange(itemList.first());
    }
}


void MainWindow::lineColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, tr("设置线条颜色"));
    if (color.isValid())
    {
        QList<QGraphicsItem*> itemList = m_scene->selectedItems();
        if (itemList.isEmpty())
            return;
        itemLineColor(m_scene, color);
        emit itemsChange(itemList.first());
    }
}

void MainWindow::lineSize(QAction* action)
{
    //qDebug() << action->data() << endl;
    qreal lineSize = action->data().toInt();
    QList<QGraphicsItem*> itemList = m_scene->selectedItems();
    if (itemList.isEmpty())
        return;
    itemLineSize(m_scene, lineSize);
    emit itemsChange(itemList.first());

}

void MainWindow::addShape()
{
    if (sender() == m_pRectAct)
        m_scene->setModel(DrawScene::Model::enRectItem);
    if (sender() == m_pLineAct)
        m_scene->setModel(DrawScene::Model::enLineItem);
    if (sender() == m_pEllipseAct)
        m_scene->setModel(DrawScene::Model::enEllipseItem);
    if (sender() == m_pRotateAct)
        m_scene->setRotateState(true);
}

void MainWindow::select1()
{
    m_scene->setModel(DrawScene::Model::enBearItem);
}

void MainWindow::select2()
{
    m_scene->setModel(DrawScene::Model::enStarItem);
}

void MainWindow::select3()
{
    m_scene->setModel(DrawScene::Model::enCartoonFace);
}

void MainWindow::onItemsChange(QGraphicsItem* item)
{
    //qDebug() << " MainWindow::itemSelected" << item << endl;
    emit itemsChange(item);
}

void MainWindow::onValuesChange()
{
    m_scene->update();
}

void MainWindow::itemAdded(QGraphicsItem* item)
{
    QUndoCommand* addCommand = new AddShapeCommand(item, m_scene);
    m_undoStack->push(addCommand);
}

void MainWindow::itemRotate(QGraphicsItem* item, const qreal oldAngle)
{
    QUndoCommand* rotateCommand = new RotateShapeCommand(item, oldAngle);
    m_undoStack->push(rotateCommand);
}

void MainWindow::itemLineSize(QGraphicsScene* scene, const qreal newSize)
{
    QUndoCommand* lineSizeCommand = new LineSizeCommand(scene, newSize);
    m_undoStack->push(lineSizeCommand);
}

void MainWindow::itemLineColor(QGraphicsScene* scene, const QColor newColor)
{
    QUndoCommand* lineColorCommand = new LineColorCommand(scene, newColor);
    m_undoStack->push(lineColorCommand);
}

void MainWindow::itemBrushColor(QGraphicsScene* scene, const QColor newColor)
{
    QUndoCommand* brushColorCommand = new BrushColorCommand(scene, newColor);
    m_undoStack->push(brushColorCommand);
}

void MainWindow::itemResize(QGraphicsItem* item, const QRectF rect)
{
    QUndoCommand* resizeShapeCommand = new ResizeShapeCommand(item, rect);
    m_undoStack->push(resizeShapeCommand);
}

void MainWindow::itemMoved(QList<QGraphicsItem*> item)
{
    QUndoCommand* moveShapeCommand = new MoveShapeCommand(item);
    m_undoStack->push(moveShapeCommand);
}

void MainWindow::createActions()
{
    //File
    m_pNewAct = new QAction(tr("&New"), this);
    m_pNewAct->setIcon(QIcon(":/icons/new.png"));
    m_pNewAct->setShortcuts(QKeySequence::New);
    m_pNewAct->setStatusTip(tr("Create a new file"));
    connect(m_pNewAct, SIGNAL(triggered()), this, SLOT(newFile()));

    m_pOpenAct = new QAction(tr("&Open..."), this);
    m_pOpenAct->setIcon(QIcon(":/icons/open.png"));
    m_pOpenAct->setShortcuts(QKeySequence::Open);
    m_pOpenAct->setStatusTip(tr("Open an existing file"));
    connect(m_pOpenAct, SIGNAL(triggered()), this, SLOT(open()));

    m_pSaveAct = new QAction(tr("&Save"), this);
    m_pSaveAct->setIcon(QIcon(":/icons/save.png"));
    m_pSaveAct->setShortcuts(QKeySequence::Save);
    m_pSaveAct->setStatusTip(tr("Save the document to disk"));
    connect(m_pSaveAct, SIGNAL(triggered()), this, SLOT(save()));

    m_pExitAct = new QAction(tr("E&xit"), this);
    m_pExitAct->setShortcuts(QKeySequence::Quit);
    m_pExitAct->setStatusTip(tr("Exit the application"));
    connect(m_pExitAct, SIGNAL(triggered()), this, SLOT(close()));

    //Edit
    m_pDeleteAct = new QAction(tr("&Delete"), this);
    m_pDeleteAct->setShortcut(QKeySequence::Delete);
    connect(m_pDeleteAct, SIGNAL(triggered()), this, SLOT(deleteItem()));

    m_pUndoAct = m_undoStack->createUndoAction(this, tr("undo"));
    m_pUndoAct->setIcon(QIcon(":/icons/undo.png"));
    m_pUndoAct->setShortcuts(QKeySequence::Undo);

    m_pRedoAct = m_undoStack->createRedoAction(this, tr("redo"));
    m_pRedoAct->setIcon(QIcon(":/icons/redo.png"));
    m_pRedoAct->setShortcuts(QKeySequence::Redo);

    m_pCopyAct = new QAction(QIcon(":/icons/copy.png"), tr("copy"), this);
    m_pCopyAct->setShortcut(QKeySequence::Copy);
    connect(m_pCopyAct, SIGNAL(triggered()), this, SLOT(on_copy()));

    m_pPasteAct = new QAction(QIcon(":/icons/paste.png"), tr("paste"), this);
    m_pPasteAct->setShortcut(QKeySequence::Paste);
    m_pPasteAct->setEnabled(true);
    connect(m_pPasteAct, SIGNAL(triggered()), this, SLOT(on_paste()));

    m_pCutAct = new QAction(QIcon(":/icons/cut.png"), tr("cut"), this);
    m_pCutAct->setShortcut(QKeySequence::Cut);
    connect(m_pCutAct, SIGNAL(triggered()), this, SLOT(on_cut()));

    //color
    m_pFillColor = new QAction("fill", this);
    m_pFillColor->setIcon(QIcon(":/icons/fillColor.png"));
    m_pFillColor->setStatusTip(tr("Color fill"));
    //m_pFillColor->setEnabled(false);
    connect(m_pFillColor, SIGNAL(triggered()), this, SLOT(fillColor()));

    m_pLineColor = new QAction("LineColor", this);
    m_pLineColor->setIcon(QIcon(":/icons/lineColor.png"));
    m_pLineColor->setStatusTip(tr("Line color"));
    // m_pLineColor->setEnabled(false);
    connect(m_pLineColor, SIGNAL(triggered()), this, SLOT(lineColor()));

    m_menu = createLineSizeMenu();

    m_pLineSize = new QToolButton(this);
    m_pLineSize->setMenu(m_menu);
    m_pLineSize->setIcon(QIcon(":/icons/lineSize.png"));
    m_pLineSize->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_pLineSize->setPopupMode(QToolButton::InstantPopup);
    m_pLineSize->setStatusTip(tr("Line color"));
    //m_pLineSize->setEnabled(false);
    //connect(m_pLineSize, SIGNAL(triggered()), this, SLOT(lineSize()));

    //draw
    m_pEllipseAct = new QAction(QIcon(":/icons/ellipse.png"), tr("rect"), this);
    //m_pEllipseAct->setCheckable(true);
    connect(m_pEllipseAct, SIGNAL(triggered()), this, SLOT(addShape()));

    m_pLineAct = new QAction(QIcon(":/icons/line.png"), tr("line"), this);
    //m_pLineAct->setCheckable(true);
    connect(m_pLineAct, SIGNAL(triggered()), this, SLOT(addShape()));

    m_pRectAct = new QAction(QIcon(":/icons/rectangle.png"), tr("ellipse"), this);
    //m_pRectAct->setCheckable(true);
    connect(m_pRectAct, SIGNAL(triggered()), this, SLOT(addShape()));

    m_pRotateAct = new QAction(QIcon(":/icons/rotate.png"), tr("rotate"), this);
    //m_pRotateAct->setCheckable(true);
    connect(m_pRotateAct, SIGNAL(triggered()), this, SLOT(addShape()));

    QActionGroup* drawActGroup = new QActionGroup(this);
    drawActGroup->addAction(m_pRectAct);
    drawActGroup->addAction(m_pLineAct);
    drawActGroup->addAction(m_pEllipseAct);
    drawActGroup->addAction(m_pRotateAct);

}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_pNewAct);
    fileMenu->addAction(m_pOpenAct);
    fileMenu->addAction(m_pSaveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(m_pExitAct);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(m_pUndoAct);
    editMenu->addAction(m_pRedoAct);
    editMenu->addAction(m_pCutAct);
    editMenu->addAction(m_pCopyAct);
    editMenu->addAction(m_pPasteAct);
    editMenu->addAction(m_pDeleteAct);

}

void MainWindow::createToolbars()
{
    //创建文件工具栏
    m_pFileToolBar = addToolBar(tr("File"));
    m_pFileToolBar->addAction(m_pSaveAct);
    m_pFileToolBar->addAction(m_pOpenAct);
    m_pFileToolBar->addAction(m_pNewAct);

    //创建编辑工具栏
    m_pEditToolBar = addToolBar(tr("edit"));
    m_pEditToolBar->setIconSize(QSize(24, 24));
    m_pEditToolBar->addAction(m_pCopyAct);
    m_pEditToolBar->addAction(m_pPasteAct);
    m_pEditToolBar->addAction(m_pCutAct);
    m_pEditToolBar->addAction(m_pUndoAct);
    m_pEditToolBar->addAction(m_pRedoAct);

    //创建Color工具栏
    m_pColorToolBar = addToolBar(tr("Color"));
    m_pColorToolBar->addAction(m_pFillColor);
    m_pColorToolBar->addAction(m_pLineColor);
    m_pColorToolBar->addWidget(m_pLineSize);

    //创建draw工具栏
    m_pDrawToolBar = addToolBar(tr("draw"));
    m_pDrawToolBar->addAction(m_pRectAct);
    m_pDrawToolBar->addAction(m_pLineAct);
    m_pDrawToolBar->addAction(m_pEllipseAct);
    m_pDrawToolBar->addAction(m_pRotateAct);
}


QScrollArea* MainWindow::createSelectArea(QWidget* widget)
{
    //垂直布局
    QVBoxLayout* verPlay = new QVBoxLayout;
    QScrollArea* scroll = new QScrollArea(widget);
    //scroll->setFixedSize(widget->width() / 5 , widget->height());

    //QWidget *widget=new QWidget(scroll);
    QGridLayout* griPlay = new QGridLayout;
    QGroupBox* butGroup = new QGroupBox("SVG图形", scroll);
    butGroup->setStyleSheet("margin:5px;\
                            font-family:宋体;\
                            font:bold 14px;\
");
    verPlay->addWidget(butGroup);
    verPlay->addStretch();

    QPushButton* but1 = new QPushButton(QIcon(":/icons/bear.png"), "", scroll);
    but1->setIconSize(QSize(50, 50));
    but1->setStyleSheet("QPushButton{\
						background-color: #e3e3e3;\
						border: 1px solid;\
						border-color: #c6c6c6;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                        "QPushButton:hover{\
						background-color: #e3e3e3;\
						border: 1px solid;\
						border-color: #a6a6a6;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                        "QPushButton:pressed{\
						background-color: #f8f8f8;\
						border: 1px solid;\
						border-color: #8f8f6b;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                       );
    griPlay->addWidget(but1, 0, 0);
    connect(but1, SIGNAL(clicked()), this, SLOT(select1()));

    QPushButton* but2 = new QPushButton(QIcon(":/icons/star.png"), "", scroll);
    but2->setIconSize(QSize(50, 50));
    but2->setStyleSheet("QPushButton{\
						background-color: #e3e3e3;\
						border: 1px solid;\
						border-color: #c6c6c6;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                        "QPushButton:hover{\
						background-color: #e3e3e3;\
						border: 1px solid;\
						border-color: #a6a6a6;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                        "QPushButton:pressed{\
						background-color: #f8f8f8;\
						border: 1px solid;\
						border-color: #8f8f6b;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                       );
    griPlay->addWidget(but2, 0, 1);
    connect(but2, SIGNAL(clicked()), this, SLOT(select2()));

    QPushButton* but3 = new QPushButton(QIcon(":/icons/CartoonFace.png"), "", scroll);
    but3->setIconSize(QSize(50, 50));
    but3->setStyleSheet("QPushButton{\
						background-color: #e3e3e3;\
						border: 1px solid;\
						border-color: #c6c6c6;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                        "QPushButton:hover{\
						background-color: #e3e3e3;\
						border: 1px solid;\
						border-color: #a6a6a6;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                        "QPushButton:pressed{\
						background-color: #f8f8f8;\
						border: 1px solid;\
						border-color: #8f8f6b;\
						padding-top: 4px;\
						padding-bottom: 4px;\
					}"
                       );
    griPlay->addWidget(but3, 1, 0);
    connect(but3, SIGNAL(clicked()), this, SLOT(select3()));

    scroll->setLayout(verPlay);
    butGroup->setLayout(griPlay);
    return scroll;
}

void MainWindow::createFunctionalArea()
{
    QWidget* widget = new QWidget(this);
    //widget->setFixedSize(this->width(), this->height());
    //水平布局
    QHBoxLayout* horPlay = new QHBoxLayout;
    horPlay->setSpacing(2);
    horPlay->setMargin(5);

    m_pGraphicsSelect = createSelectArea(widget);
    horPlay->addWidget(m_pGraphicsSelect);

    m_scene = new DrawScene;
    m_scene->setSceneRect(0, 0, 1920, 1080);
    m_pEditView = new DrawView(m_scene, widget);
    m_pEditView->show();
    connect(m_scene, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
    //m_pEditView->setFixedSize(widget->width() / 5 * 3, widget->height());
    horPlay->addWidget(m_pEditView);

    //创建垂直布局（右侧）
    QVBoxLayout* verPaly = new QVBoxLayout;
    verPaly->setSpacing(2);
    verPaly->addWidget(m_undoView, 1);

    m_pGraPro = new GraphicsProperty(widget);
    connect(this, SIGNAL(itemsChange(QGraphicsItem*)), m_pGraPro, SLOT(onItemsChange(QGraphicsItem*)));
    connect(m_pGraPro, SIGNAL(valuesChange()), this, SLOT(onValuesChange()));
    //m_pGraPro->setFixedSize(widget->width() / 5 , widget->height());
    m_pGraPro->show();

    verPaly->addWidget(m_pGraPro, 1);
    horPlay->addLayout(verPaly);

    horPlay->setStretchFactor(m_pGraphicsSelect, 1);
    horPlay->setStretchFactor(m_pGraPro, 1);
    horPlay->setStretchFactor(m_pEditView, 3);
    widget->setLayout(horPlay);
    setCentralWidget(widget);
}

QMenu* MainWindow::createLineSizeMenu()
{
    QMenu* menu = new QMenu(this);
    QAction* p = nullptr;
    QActionGroup* lineSizeGroup = new QActionGroup(this);
    for (int i = 0; i < 10; i++)
    {
        p = new QAction(tr("&%1px").arg(i + 1), this);
        p->setData(i + 1);
        menu->addAction(p);
        lineSizeGroup->addAction(p);
    }
    connect(lineSizeGroup, SIGNAL(triggered(QAction*)), this, SLOT(lineSize(QAction*)));
    return menu;
}

void MainWindow::relationTo() const
{
    connect(m_scene, SIGNAL(itemAdded(QGraphicsItem*)), this, SLOT(itemAdded(QGraphicsItem*)));
    connect(m_scene, SIGNAL(itemRotate(QGraphicsItem*, const qreal)), this, SLOT(itemRotate(QGraphicsItem*, const qreal)));
    connect(m_scene, SIGNAL(itemResize(QGraphicsItem*, const QRectF)), this, SLOT(itemResize(QGraphicsItem*, const QRectF)));
    connect(m_scene, SIGNAL(itemMoved(QList<QGraphicsItem*>)), this, SLOT(itemMoved(QList<QGraphicsItem*>)));

    connect(m_pGraPro, SIGNAL(itemRotate(QGraphicsItem*, const qreal)), this, SLOT(itemRotate(QGraphicsItem*, const qreal)));
    connect(m_pGraPro, SIGNAL(itemLineColor(QGraphicsScene*, const QColor)), this, SLOT(itemLineColor(QGraphicsScene*, const QColor)));
    connect(m_pGraPro, SIGNAL(itemLineSize(QGraphicsScene*, const qreal)), this, SLOT(itemLineSize(QGraphicsScene*, const qreal)));
    connect(m_pGraPro, SIGNAL(itemBrushColor(QGraphicsScene*, const QColor)), this, SLOT(itemBrushColor(QGraphicsScene*, const QColor)));
    connect(m_pGraPro, SIGNAL(itemMoved(QList<QGraphicsItem*>)), this, SLOT(itemMoved(QList<QGraphicsItem*>)));
    connect(m_pGraPro, SIGNAL(itemResize(QGraphicsItem*, const QRectF)), this, SLOT(itemResize(QGraphicsItem*, const QRectF)));
}

