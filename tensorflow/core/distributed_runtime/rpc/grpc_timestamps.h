#ifndef TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_GRPC_TIMESTAMPS_H_
#define TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_GRPC_TIMESTAMPS_H_

#include "grpcpp/timestamps.h"

namespace tensorflow {

void process_timestamps(::grpc::TimestampsArgs *arg,
    ::grpc::Timestamps *timestamps);

}  // namespace tensorflow

#endif  // TENSORFLOW_CORE_DISTRIBUTED_RUNTIME_RPC_GRPC_TIMESTAMPS_H_
