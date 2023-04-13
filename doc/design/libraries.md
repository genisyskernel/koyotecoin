# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libkoyotecoin_cli*         | RPC client functionality used by *koyotecoin-cli* executable |
| *libkoyotecoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libkoyotecoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libkoyotecoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libkoyotecoin_node* and *libkoyotecoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libkoyotecoinconsensus*    | Shared library build of static *libkoyotecoin_consensus* library |
| *libkoyotecoin_kernel*      | Consensus engine and support library used for validation by *libkoyotecoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libkoyotecoinqt*           | GUI functionality used by *koyotecoin-qt* and *koyotecoin-gui* executables |
| *libkoyotecoin_ipc*         | IPC functionality used by *koyotecoin-node*, *koyotecoin-wallet*, *koyotecoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libkoyotecoin_node*        | P2P and RPC server functionality used by *koyotecoind* and *koyotecoin-qt* executables. |
| *libkoyotecoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libkoyotecoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libkoyotecoin_wallet*      | Wallet functionality used by *koyotecoind* and *koyotecoin-wallet* executables. |
| *libkoyotecoin_wallet_tool* | Lower-level wallet functionality used by *koyotecoin-wallet* executable. |
| *libkoyotecoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *koyotecoind* and *koyotecoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libkoyotecoin_consensus* and *libkoyotecoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libkoyotecoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libkoyotecoin_node* code lives in `src/node/` in the `node::` namespace
  - *libkoyotecoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libkoyotecoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libkoyotecoin_util* code lives in `src/util/` in the `util::` namespace
  - *libkoyotecoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "linear" }}}%%

graph TD;

koyotecoin-cli[koyotecoin-cli]-->libkoyotecoin_cli;

koyotecoind[koyotecoind]-->libkoyotecoin_node;
koyotecoind[koyotecoind]-->libkoyotecoin_wallet;

koyotecoin-qt[koyotecoin-qt]-->libkoyotecoin_node;
koyotecoin-qt[koyotecoin-qt]-->libkoyotecoinqt;
koyotecoin-qt[koyotecoin-qt]-->libkoyotecoin_wallet;

koyotecoin-wallet[koyotecoin-wallet]-->libkoyotecoin_wallet;
koyotecoin-wallet[koyotecoin-wallet]-->libkoyotecoin_wallet_tool;

libkoyotecoin_cli-->libkoyotecoin_common;
libkoyotecoin_cli-->libkoyotecoin_util;

libkoyotecoin_common-->libkoyotecoin_util;
libkoyotecoin_common-->libkoyotecoin_consensus;

libkoyotecoin_kernel-->libkoyotecoin_consensus;
libkoyotecoin_kernel-->libkoyotecoin_util;

libkoyotecoin_node-->libkoyotecoin_common;
libkoyotecoin_node-->libkoyotecoin_consensus;
libkoyotecoin_node-->libkoyotecoin_kernel;
libkoyotecoin_node-->libkoyotecoin_util;

libkoyotecoinqt-->libkoyotecoin_common;
libkoyotecoinqt-->libkoyotecoin_util;

libkoyotecoin_wallet-->libkoyotecoin_common;
libkoyotecoin_wallet-->libkoyotecoin_util;

libkoyotecoin_wallet_tool-->libkoyotecoin_util;
libkoyotecoin_wallet_tool-->libkoyotecoin_wallet;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class koyotecoin-qt,koyotecoind,koyotecoin-cli,koyotecoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libkoyotecoin_wallet* and *libkoyotecoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code still is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libkoyotecoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libkoyotecoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libkoyotecoin_common* should serve a similar function as *libkoyotecoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libkoyotecoin_util* and *libkoyotecoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for koyotecoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libkoyotecoin_kernel* is not supposed to depend on *libkoyotecoin_common*, only *libkoyotecoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libkoyotecoin_kernel* should only depend on *libkoyotecoin_util* and *libkoyotecoin_consensus*.

- The only thing that should depend on *libkoyotecoin_kernel* internally should be *libkoyotecoin_node*. GUI and wallet libraries *libkoyotecoinqt* and *libkoyotecoin_wallet* in particular should not depend on *libkoyotecoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libkoyotecoin_consensus*, *libkoyotecoin_common*, and *libkoyotecoin_util*, instead of *libkoyotecoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libkoyotecoinqt*, *libkoyotecoin_node*, *libkoyotecoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libkoyotecoin_node* to *libkoyotecoin_kernel* as part of [The libkoyotecoinkernel Project #24303](https://github.com/koyotecoin/koyotecoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/koyotecoin/koyotecoin/issues/15732)
