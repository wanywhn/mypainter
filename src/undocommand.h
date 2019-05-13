#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H


#include <QUndoCommand>
#include <QImage>

class PaintWidget;
class UndoCommand : public QUndoCommand
{
public:
    UndoCommand(const QImage* img, PaintWidget &imgArea, QUndoCommand *parent = 0);

    virtual void undo();
    virtual void redo();
private:
    QImage mPrevImage;
    QImage mCurrImage;
    PaintWidget & mImageArea;
};

class CommandInterface{
public:
    void makeUndoCommand(PaintWidget &paintWidget);

};

#endif // UNDOCOMMAND_H
