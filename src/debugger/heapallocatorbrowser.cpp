//
// allwatch.cpp - All Watch
//
// by Jean-Paul Mari
//
// JPM = Jean-Paul Mari <djipi.mari@gmail.com>
//
// Who  When        What
// ---  ----------  -----------------------------------------------------------
// JPM  01/08/2017  Created this file
//

// STILL TO DO:
//

#include "debugger/heapallocatorbrowser.h"
#include "memory.h"
#include "debugger/DBGManager.h"
#include "m68000/m68kinterface.h"


HeapAllocatorBrowserWindow::HeapAllocatorBrowserWindow(QWidget * parent/*= 0*/) : QWidget(parent, Qt::Dialog),
layout(new QVBoxLayout), text(new QTextBrowser),
//	layout(new QVBoxLayout), text(new QLabel),
//	refresh(new QPushButton(tr("Refresh"))),
//	address(new QLineEdit),
//	go(new QPushButton(tr("Go"))),
//	memBase(0),
//	NbWatch(0),
//	PtrWatchInfo(NULL)
Adr(0)
//PtrAdr(NULL)
{
	setWindowTitle(tr("Heap Allocation"));

//	address->setInputMask("hhhhhh");
//	QHBoxLayout * hbox1 = new QHBoxLayout;
//	hbox1->addWidget(refresh);
//	hbox1->addWidget(address);
//	hbox1->addWidget(go);

	// Need to set the size as well...
//	resize(560, 480);

	QFont fixedFont("Lucida Console", 8, QFont::Normal);
//	QFont fixedFont("", 8, QFont::Normal);
	fixedFont.setStyleHint(QFont::TypeWriter);
	text->setFont(fixedFont);
////	layout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(layout);

	layout->addWidget(text);
//	layout->addWidget(refresh);
//	layout->addLayout(hbox1);

//	connect(refresh, SIGNAL(clicked()), this, SLOT(RefreshContents()));
//	connect(go, SIGNAL(clicked()), this, SLOT(GoToAddress()));
}


//
HeapAllocatorBrowserWindow::~HeapAllocatorBrowserWindow(void)
{
//	NbWatch = 0;
//	free(PtrAdr);
}


//
void HeapAllocatorBrowserWindow::RefreshContents(void)
{
	char string[1024];
//	char buf[64];
	QString HA;
	size_t Adr68K;
	size_t Error = 0;
	HeapAllocation HeapAllocation;

	if (isVisible())
	{
		if (Adr68K = Adr)
		{
			do
			{
				if ((Adr68K >= 0x4000) && (Adr68K < 0x200000))
				{
					memcpy(&HeapAllocation, &jaguarMainRAM[Adr68K], sizeof(HeapAllocation));

					if (HeapAllocation.size = ((HeapAllocation.size & 0xff) << 24) + ((HeapAllocation.size & 0xff00) << 8) + ((HeapAllocation.size & 0xff0000) >> 8) + ((HeapAllocation.size & 0xff000000) >> 24))
					{
						if (HeapAllocation.size <= (0x200000 - 0x4000))
						{
							if ((HeapAllocation.used = ((HeapAllocation.used & 0xff) << 8) + ((HeapAllocation.used & 0xff00) >> 8)) <= 1)
							{
								HeapAllocation.nextalloc = ((HeapAllocation.nextalloc & 0xff) << 24) + ((HeapAllocation.nextalloc & 0xff00) << 8) + ((HeapAllocation.nextalloc & 0xff0000) >> 8) + ((HeapAllocation.nextalloc & 0xff000000) >> 24);

								if ((HeapAllocation.nextalloc >= 0x4000) && (HeapAllocation.nextalloc < 0x200000))
								{
									sprintf(string, "0x%06x | 0x%06x (%i) | %s | 0x%06x<br>", Adr68K, HeapAllocation.size - sizeof(HeapAllocation), HeapAllocation.size - sizeof(HeapAllocation), HeapAllocation.used ? "Allocated" : "Free", HeapAllocation.nextalloc);
									Adr68K = HeapAllocation.nextalloc;
								}
								else
								{
									sprintf(string, "<br><font color='#ff0000'><b>Unable to determine the next memory allocation</b></font>");
									Error = 1;
								}
							}
							else
							{
								sprintf(string, "<br><font color='#ff0000'><b>Unable to determine if the allocated memory is used or not</b></font>");
								Error = 2;
							}
						}
						else
						{
							sprintf(string, "<br><font color='#ff0000'><b>Memory bloc size has a problem</b></font>");
							Error = 3;
						}
					}
					else
					{
						sprintf(string, "<br><font color='#0000ff'><b>Memory allocations browsing successfully completed</b></font>");
					}
				}
				else
				{
					sprintf(string, "<br><font color='#ff0000'><b>Memory allocations may have a problem</b></font>");
					Error = 4;
				}

				HA += QString(string);

			} while (HeapAllocation.size && !Error);
		}
		else
		{
			if (Adr = DBGManager_GetAdrFromSymbolName((char *)"__HeapBase"))
			{
				if (Adr68K = DBGManager_GetExternalVariableAdrFromName((char *)"alloc"))
				{
					if (!(Adr68K = (jaguarMainRAM[Adr68K] << 24) + (jaguarMainRAM[Adr68K + 1] << 16) + (jaguarMainRAM[Adr68K + 2] << 8) + (jaguarMainRAM[Adr68K + 3])) || ((Adr68K < 0x4000) || (Adr68K >= 0x200000)))
					{
						sprintf(string, "<font color='#ff0000'><b>Memory allocator not yet initialised</b></font>");
						Adr = 0;
					}
					else
					{
						return RefreshContents();
						//sprintf(string, "<font color='#0000ff'><b>Memory allocator has been initialised</b></font>");
					}
				}
				else
				{
					sprintf(string, "<font color='#ff0000'><b>Memory allocator is not compatible</b></font>");
					Adr = 0;
				}
			}
			else
			{
				sprintf(string, "<font color='#ff0000'><b>Memory allocator doesn't exist</b></font>");
			}

			HA += QString(string);
		}

		text->clear();
		text->setText(HA);
	}
}


#if 0
void HeapAllocatorBrowserWindow::keyPressEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_Escape)
		hide();
	else if (e->key() == Qt::Key_PageUp)
	{
		memBase -= 480;

		if (memBase < 0)
			memBase = 0;

		RefreshContents();
	}
	else if (e->key() == Qt::Key_PageDown)
	{
		memBase += 480;

		if (memBase > (0x200000 - 480))
			memBase = 0x200000 - 480;

		RefreshContents();
	}
	else if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Minus)
	{
		memBase -= 16;

		if (memBase < 0)
			memBase = 0;

		RefreshContents();
	}
	else if (e->key() == Qt::Key_Down || e->key() == Qt::Key_Equal)
	{
		memBase += 16;

		if (memBase > (0x200000 - 480))
			memBase = 0x200000 - 480;

		RefreshContents();
	}
}
#endif


#if 0
void HeapAllocatorBrowserWindow::GoToAddress(void)
{
	bool ok;
	QString newAddress = address->text();
	memBase = newAddress.toUInt(&ok, 16);
	RefreshContents();
}
#endif
