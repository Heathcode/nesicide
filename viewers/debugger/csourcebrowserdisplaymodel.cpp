#include "csourcebrowserdisplaymodel.h"

#include "dbg_cnes.h"
#include "dbg_cnesrom.h"
#include "dbg_cnes6502.h"
#include "cnesicideproject.h"

#include "cbreakpointinfo.h"

#include "pasm_lib.h"

#include "cmarker.h"

#include "main.h"

#include <QIcon>

static char modelStringBuffer [ 2048 ];

enum
{
   Column_LineNumber = 0,
   Column_Decoration,
   Column_Source,
   Column_Max
};

CSourceBrowserDisplayModel::CSourceBrowserDisplayModel(QObject*)
{
   m_sourceFilename = "?";
}

CSourceBrowserDisplayModel::~CSourceBrowserDisplayModel()
{
}

QVariant CSourceBrowserDisplayModel::data(const QModelIndex& index, int role) const
{
   CBreakpointInfo* pBreakpoints = nesGetBreakpointDatabase();
   int idx;
   int32_t addr;
   int32_t absAddr;
   CMarker* markers = nesGetExecutionMarkerDatabase();
   MarkerSetInfo* pMarker;

   if (!index.isValid())
   {
      return QVariant();
   }

   addr = pasm_get_source_addr_by_linenum_and_file(index.row()+1,this->m_sourceFilename.toAscii().constData());

   absAddr = pasm_get_source_absolute_addr_by_linenum_and_file(index.row()+1,this->m_sourceFilename.toAscii().constData());

   if ( role == Qt::ToolTipRole )
   {
      if ( addr != -1 )
      {
         if ( index.column() > Column_Decoration )
         {
            CNESDBG::CODEBROWSERTOOLTIP(TOOLTIP_INFO,addr,modelStringBuffer);
            return QVariant(modelStringBuffer);
         }
      }
   }

   if ( (role == Qt::BackgroundRole) && (index.column() == Column_Decoration)  )
   {
      for ( idx = 0; idx < markers->GetNumMarkers(); idx++ )
      {
         pMarker = markers->GetMarker(idx);

         if ( (pMarker->state == eMarkerSet_Started) ||
               (pMarker->state == eMarkerSet_Complete) )
         {
            if ( (absAddr >= pMarker->startAbsAddr) &&
                  (absAddr <= pMarker->endAbsAddr) )
            {
               return QColor(pMarker->red,pMarker->green,pMarker->blue);
            }
         }
      }

      return QVariant();
   }

   if ( (role == Qt::DecorationRole) && (index.column() == Column_Decoration) )
   {
      for ( idx = 0; idx < pBreakpoints->GetNumBreakpoints(); idx++ )
      {
         BreakpointInfo* pBreakpoint = pBreakpoints->GetBreakpoint(idx);

         if ( (pBreakpoint->enabled) &&
               (pBreakpoint->type == eBreakOnCPUExecution) &&
               (pBreakpoint->item1 <= addr) &&
               ((absAddr == -1) || (absAddr == pBreakpoint->item1Absolute)) &&
               (pBreakpoint->item2 >= addr) )
         {
            if ( addr == nesGetCPUProgramCounterOfLastSync() )
            {
               return QIcon(":/resources/22_execution_break.png");
            }
            else
            {
               return QIcon(":/resources/22_breakpoint.png");
            }
         }
         else if ( (!pBreakpoint->enabled) &&
                   (pBreakpoint->type == eBreakOnCPUExecution) &&
                   (pBreakpoint->item1 <= addr) &&
                   ((absAddr == -1) || (absAddr == pBreakpoint->item1Absolute)) &&
                   (pBreakpoint->item2 >= addr) )
         {
            if ( addr == nesGetCPUProgramCounterOfLastSync() )
            {
               return QIcon(":/resources/22_execution_break_disabled.png");
            }
            else
            {
               return QIcon(":/resources/22_breakpoint_disabled.png");
            }
         }
      }

      if ( absAddr == nesGetAbsoluteAddressFromAddress(nesGetCPUProgramCounterOfLastSync()) )
      {
         return QIcon(":/resources/22_execution_pointer.png");
      }
   }

   if (role != Qt::DisplayRole)
   {
      return QVariant();
   }

   switch ( index.column() )
   {
      case Column_LineNumber:
         sprintf ( modelStringBuffer, "%d", index.row()+1 );
         return QVariant(modelStringBuffer);
         break;
      case Column_Decoration:
         return QVariant();
         break;
      case Column_Source:
         return m_source.at ( index.row() );
         break;
   }

   return QVariant();
}

Qt::ItemFlags CSourceBrowserDisplayModel::flags(const QModelIndex&) const
{
   Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
   return flags;
}

QVariant CSourceBrowserDisplayModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if (role != Qt::DisplayRole)
   {
      return QVariant();
   }

   if ( orientation == Qt::Horizontal )
   {
      switch ( section )
      {
         case Column_LineNumber:
            return "#";
            break;
         case Column_Decoration:
            return "!";
            break;
         case Column_Source:
            return QVariant(m_sourceFilename);
            break;
      }
   }

   return  QVariant();
}

QModelIndex CSourceBrowserDisplayModel::index(int row, int column, const QModelIndex&) const
{
   if ( (row >= 0) && (column >= 0) )
   {
      return createIndex(row, column, row);
   }

   return QModelIndex();
}

int CSourceBrowserDisplayModel::rowCount(const QModelIndex&) const
{
   return m_source.count();
}

int CSourceBrowserDisplayModel::columnCount(const QModelIndex& parent) const
{
   if ( parent.isValid())
   {
      return 0;
   }

   return Column_Max;
}

void CSourceBrowserDisplayModel::update()
{
#if 0
   QStringList  source;
   QString      filetext(pasm_get_source_file_text_by_addr(nesGetAbsoluteAddressFromAddress(nesGetCPUProgramCounterOfLastSync())));
   
   source = filetext.split ( QRegExp("[\r\n]") );
   setSource(source);
   setSourceFilename(pasm_get_source_file_name_by_addr(nesGetAbsoluteAddressFromAddress(nesGetCPUProgramCounterOfLastSync())));
#endif
   emit layoutChanged();
}

void CSourceBrowserDisplayModel::force()
{
   emit layoutChanged();
}
