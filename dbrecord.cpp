#include "dbrecord.h"

DBRecord::DBRecord()
{
}

DBRecord::DBRecord(DBRecord &record)
{
	m_id = record.id();
	m_name = record.name();
	m_leftIrisTemplate = record.leftIrisTemplate();
    m_rightIrisTemplate = record.rightIrisTemplate();
    m_faceFeatureTemplate = record.faceFeatureTemplate();
    m_leftIrisPath = record.leftIrisPath();
    m_rightIrisPath = record.rightIrisPath();
    m_faceImagePath = record.faceImagePath();
}
