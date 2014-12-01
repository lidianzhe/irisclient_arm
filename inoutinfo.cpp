#include "inoutinfo.h"

InoutInfo::InoutInfo()
{

}

InoutInfo::InoutInfo(InoutInfo &inout){
    m_pid = inout.pid();
    m_deviceNo = inout.deviceNo();
    m_personId = inout.personId();
    m_cardTime = inout.cardTime();
    m_flag = inout.flag();
    m_seriesId = inout.seriesId();
    m_if_UserNo = inout.if_UserNo();
}
