#include "tensorflow/core/distributed_runtime/rpc/grpc_timestamps.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

#include "grpcpp/grpcpp.h"

namespace tensorflow {

namespace {

std::string to_string(const gpr_timespec &ts) {
  std::ostringstream oss;
  oss << ts.tv_sec;
  if (ts.tv_nsec) {
    oss << ".";
    oss.width(9);
    oss.fill('0');
    oss << ts.tv_nsec;
  }
  return oss.str();
}

std::string get_qstr(::grpc::TimestampsArgs *arg,
    ::grpc::Timestamps *timestamps) {
  std::string query(
      "INSERT INTO Timestamps (uuid, name, rpc_type, peer, seq, msg_size, "
      "sendmsg_time, scheduled_time, sent_time, received_time, acked_time) "
      "VALUES ("
  );
  query +=  "'" + arg->rpc_uuid  + "', "
          + "'" + arg->func_name + "', "
          + "'" + arg->rpc_type  + "', "
          + "'" + arg->peer      + "', "
          + std::to_string(arg->seq_no)           + ", "
          + std::to_string(arg->size)             + ", "
          + to_string(timestamps->sendmsg_time)   + ", "
          + to_string(timestamps->scheduled_time) + ", "
          + to_string(timestamps->sent_time)      + ", "
          + to_string(timestamps->received_time)  + ", "
          + to_string(timestamps->acked_time)     + ");";
  return query;
}

/*
 * -2: haven't created the dumpfile
 * -1: failed to create the dumpfile
 * >= 0: successfully created the dumpfile
 */
int dumpfd = -2;
char dumpfile[] = "tsdump.XXXXXX.sql";

}  // namespace

void process_timestamps(::grpc::TimestampsArgs *arg,
    ::grpc::Timestamps *timestamps) {
  if (dumpfd == -2) {
    dumpfd = mkostemps(dumpfile, 4, O_TRUNC);
    if (dumpfd == -1) {
      std::cerr << "[Timestamps] Failed to create " << dumpfile << std::endl;
    }
  } else if (dumpfd >= 0) {
    std::string query = get_qstr(arg, timestamps) + "\n";
    if (write(dumpfd, query.c_str(), query.size()) == -1) {
      std::cerr << "[Timestamps] Failed to write to " << dumpfile << std::endl;
    }
  }
}

}  // namespace tensorflow
