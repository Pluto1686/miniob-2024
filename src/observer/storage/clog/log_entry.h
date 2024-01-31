/* Copyright (c) 2021-2022 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by wangyunlai on 2024/01/30
//

#pragma once

#include <memory>
#include <string>
#include "common/rc.h"
#include "common/types.h"
#include "storage/clog/log_module.h"

/**
 * @brief 描述一条日志头
 * @ingroup CLog
 */
struct LogHeader final
{
  LSN     lsn;        /// 日志序列号 log sequence number
  int32_t size;       /// 日志数据大小，不包含日志头
  int32_t module_id;  /// 日志模块编号

  static const int32_t SIZE; /// 日志头大小

  std::string to_string() const;
};

/**
 * @brief 描述一条日志
 * @ingroup CLog
 */
class LogEntry
{
public:
  LogEntry() = default;
  ~LogEntry() = default;

  /**
   * @brief 由于日志数据是一个比较消耗内存的对象，所以尽量使用move语义
   */
  LogEntry(LogEntry &&other);

  LogEntry &operator= (LogEntry &&other);

public:
  /**
   * @brief 一条日志的最大大小
   */
  static int32_t max_size() { return 4 * 1024 * 1024; }
  /**
   * @brief 一条日志最大payload大小
   */
  static int32_t max_payload_size() { return max_size() - LogHeader::SIZE; }

public:
  RC init(LSN lsn, LogModule module, std::unique_ptr<char[]> data, int32_t size);

  const LogHeader &header() const { return header_; }
  const char *data() const { return data_.get(); }
  int32_t size() const { return LogHeader::SIZE + header_.size; }

  LSN lsn() const { return header_.lsn; }
  LogModule module() const { return LogModule(header_.module_id); }

public:
  std::string to_string() const;

private:
  LogHeader               header_;    /// 日志头
  std::unique_ptr<char[]> data_;      /// 日志数据
};
