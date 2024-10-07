#include <QMimeData>

#include "importmediabutton.h"

ImportMediaButton::ImportMediaButton(QWidget *parent)
	: QPushButton(parent)
{
	setAcceptDrops(true);
}

void ImportMediaButton::dragEnterEvent(QDragEnterEvent *event)
{
	setText("<Drop Media File>");
	event->acceptProposedAction();
}

void ImportMediaButton::dragLeaveEvent(QDragLeaveEvent *event)
{
	resetButton();
	event->accept();
}

void ImportMediaButton::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasUrls())
	{
		QUrl filePath = event->mimeData()->urls()[0];

		if (filePath.isLocalFile())
		{
			emit fileDropped(filePath);
		}
	}

	resetButton();
	event->acceptProposedAction();
}

void ImportMediaButton::resetButton()
{
	setText("Upload Media");
}
