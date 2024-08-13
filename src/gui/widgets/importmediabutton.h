#pragma once

#include <QPushButton>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

class ImportMediaButton : public QPushButton
{
	Q_OBJECT

public:
	ImportMediaButton(QWidget* parent = nullptr);

signals:
	void fileDropped(const QUrl& source);

private:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dragLeaveEvent(QDragLeaveEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	void resetButton();
};