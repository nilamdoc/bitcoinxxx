# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *librenaiss_cli*         | RPC client functionality used by *renaiss-cli* executable |
| *librenaiss_common*      | Home for common functionality shared by different executables and libraries. Similar to *librenaiss_util*, but higher-level (see [Dependencies](#dependencies)). |
| *librenaiss_consensus*   | Stable, backwards-compatible consensus functionality used by *librenaiss_node* and *librenaiss_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *librenaissconsensus*    | Shared library build of static *librenaiss_consensus* library |
| *librenaiss_kernel*      | Consensus engine and support library used for validation by *librenaiss_node* and also exposed as a [shared library](../shared-libraries.md). |
| *librenaissqt*           | GUI functionality used by *renaiss-qt* and *renaiss-gui* executables |
| *librenaiss_ipc*         | IPC functionality used by *renaiss-node*, *renaiss-wallet*, *renaiss-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *librenaiss_node*        | P2P and RPC server functionality used by *renaissd* and *renaiss-qt* executables. |
| *librenaiss_util*        | Home for common functionality shared by different executables and libraries. Similar to *librenaiss_common*, but lower-level (see [Dependencies](#dependencies)). |
| *librenaiss_wallet*      | Wallet functionality used by *renaissd* and *renaiss-wallet* executables. |
| *librenaiss_wallet_tool* | Lower-level wallet functionality used by *renaiss-wallet* executable. |
| *librenaiss_zmq*         | [ZeroMQ](../zmq.md) functionality used by *renaissd* and *renaiss-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *librenaiss_consensus* and *librenaiss_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`librenaiss_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *librenaiss_node* code lives in `src/node/` in the `node::` namespace
  - *librenaiss_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *librenaiss_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *librenaiss_util* code lives in `src/util/` in the `util::` namespace
  - *librenaiss_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "linear" }}}%%

graph TD;

renaiss-cli[renaiss-cli]-->librenaiss_cli;

renaissd[renaissd]-->librenaiss_node;
renaissd[renaissd]-->librenaiss_wallet;

renaiss-qt[renaiss-qt]-->librenaiss_node;
renaiss-qt[renaiss-qt]-->librenaissqt;
renaiss-qt[renaiss-qt]-->librenaiss_wallet;

renaiss-wallet[renaiss-wallet]-->librenaiss_wallet;
renaiss-wallet[renaiss-wallet]-->librenaiss_wallet_tool;

librenaiss_cli-->librenaiss_common;
librenaiss_cli-->librenaiss_util;

librenaiss_common-->librenaiss_util;
librenaiss_common-->librenaiss_consensus;

librenaiss_kernel-->librenaiss_consensus;
librenaiss_kernel-->librenaiss_util;

librenaiss_node-->librenaiss_common;
librenaiss_node-->librenaiss_consensus;
librenaiss_node-->librenaiss_kernel;
librenaiss_node-->librenaiss_util;

librenaissqt-->librenaiss_common;
librenaissqt-->librenaiss_util;

librenaiss_wallet-->librenaiss_common;
librenaiss_wallet-->librenaiss_util;

librenaiss_wallet_tool-->librenaiss_util;
librenaiss_wallet_tool-->librenaiss_wallet;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class renaiss-qt,renaissd,renaiss-cli,renaiss-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *librenaiss_wallet* and *librenaiss_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code still is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *librenaiss_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *librenaiss_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *librenaiss_common* should serve a similar function as *librenaiss_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *librenaiss_util* and *librenaiss_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for renaiss-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *librenaiss_kernel* is not supposed to depend on *librenaiss_common*, only *librenaiss_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *librenaiss_kernel* should only depend on *librenaiss_util* and *librenaiss_consensus*.

- The only thing that should depend on *librenaiss_kernel* internally should be *librenaiss_node*. GUI and wallet libraries *librenaissqt* and *librenaiss_wallet* in particular should not depend on *librenaiss_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *librenaiss_consensus*, *librenaiss_common*, and *librenaiss_util*, instead of *librenaiss_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *librenaissqt*, *librenaiss_node*, *librenaiss_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *librenaiss_node* to *librenaiss_kernel* as part of [The librenaisskernel Project #24303](https://github.com/renaiss/renaiss/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/renaiss/renaiss/issues/15732)
