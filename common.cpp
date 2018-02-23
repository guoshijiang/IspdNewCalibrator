#include "common.h"

namespace Common
{
    HandleError::HandleError()
    {

    }

    HandleError::HandleError(QString http_req_error, QString database_error, QString sport_error)
    {
        this->m_http_req_error = http_req_error;
        this->m_database_error = database_error;
        this->m_sport_error = sport_error;
    }

    HandleError::~HandleError()
    {

    }

    void HandleError::HandleHttpReqError(int err_code)
    {
        QString succ_log;
        QString succ_log_msg;
        QString fail_log;
        QString fail_log_msg;
        switch (err_code)
        {
        case SUCCESS_CODE:
            succ_log = UTF8BIT("成功");
            succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
            this->m_http_req_error = succ_log_msg;
            break;

        case NO_AVI_DATA:
            fail_log = UTF8BIT("服务端报错--没有可用的数据:");
            fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
            this->m_http_req_error = fail_log_msg;
            break;

        case REQUEST_DATA_ERR:
            fail_log = UTF8BIT("服务端报错--请求的数据格式错误:");
            fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
            this->m_http_req_error = fail_log_msg;
            break;

        case CAN_NOT_FIND_MASTERMR:
            fail_log = UTF8BIT("服务端报错--请求的数据格式错误:");
            fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
            this->m_http_req_error = fail_log_msg;
            break;

        case CAN_NOT_FINE_MR:
            fail_log = UTF8BIT("服务端报错--找不到基站:");
            fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
            this->m_http_req_error = fail_log_msg;
            break;

        case MR_INNER_TIMEOUT:
            fail_log = UTF8BIT("服务端报错--基站内部超时:");
            fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
            this->m_http_req_error = fail_log_msg;
            break;

        default:
            fail_log = UTF8BIT("服务端报错--未知类型错误:");
            fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
            this->m_http_req_error = fail_log_msg;
            break;
        }
    }
}


