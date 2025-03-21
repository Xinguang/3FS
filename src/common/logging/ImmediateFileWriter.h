/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <folly/Range.h>
#include <folly/logging/LogWriter.h>

#include "IWritableFile.h"

namespace hf3fs::logging {

/**
 * A LogWriter implementation that immediately writes to a file descriptor when
 * it is invoked.
 *
 * The downside of this class is that logging I/O occurs directly in your
 * normal program threads, so that logging I/O may block or slow down normal
 * processing.
 *
 * However, one benefit of this class is that log messages are written out
 * immediately, so if your program crashes, all log messages generated before
 * the crash will have already been written, and no messages will be lost.
 */
class ImmediateFileWriter : public folly::LogWriter {
 public:
  /**
   * Construct an ImmediateFileWriter that writes to the specified File object.
   */
  explicit ImmediateFileWriter(std::shared_ptr<IWritableFile> file);

  using LogWriter::writeMessage;
  void writeMessage(folly::StringPiece buffer, uint32_t flags = 0) override;
  void flush() override;

  /**
   * Returns true if the output steam is a tty.
   */
  bool ttyOutput() const override { return file_->ttyOutput(); }

 private:
  ImmediateFileWriter(ImmediateFileWriter const &) = delete;
  ImmediateFileWriter &operator=(ImmediateFileWriter const &) = delete;

  std::shared_ptr<IWritableFile> file_;
};
}  // namespace hf3fs::logging
