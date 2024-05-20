// 0x6080604052348015600e575f80fd5b5060a58061001b5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea2646970667358221220c276cfaa26a978c7cbd6472fe8f6998f4608429a6fb1e97f12f1b1fd0b52111d64736f6c63430008140033


// 1. Contract creation code
// Free memory pointer
PUSH1 0x80          // [0x80]
PUSH1 0x40          // [0x40, 0x80]
MSTORE              // []           // Memory 0x40 -> 0x80

// What is this?
// This is from the solidity compiler. He knows it has a nonpayable constructor so it revert
// If some value is send on creation
CALLVALUE           // [msg.value]
DUP1                // [msg.value, msg.value]
ISZERO              // [msg.value == 0, msg.value]
PUSH1 0x0e          // [0x0e, msg.value == 0, msg.value]
JUMPI               // [msg.value]  // If msg.value == 0, jump to 0x0e PC/JumpDest
PUSH0               // [0x00, msg.value]
DUP1                // [0x00, 0x00, msg.value]
REVERT              // [msg.value] If someone send value to this call, revert. Otherwise, jump

// JumpDest if msg.value == 0
JUMPDEST            // [msg.value]
POP                 // []           // Pop msg.value
PUSH1 0xa5          // [0xa5]
DUP1                // [0xa5, 0xa5]
PUSH2 0x001b        // [0x001b, 0xa5, 0xa5]
PUSH0               // [0x00, 0x001b, 0xa5, 0xa5]
CODECOPY            // [0xa5]  Memory: [runtime code] starts at 0x001b and ends at 0x001b + 0xa5
PUSH0               // [0x00, 0xa5]
RETURN              // []           Offsets: 0x00 Size: 0xa5
INVALID             // []           This is from solidity compiler. Marks the end of the contract creation code
// Now the runtime code is copied to the blockchain and the contract is created

// 2. Runtime code
// Entry point of all calls
// Free memory pointer
PUSH1 0x80
PUSH1 0x40
MSTORE

// Checking msg.value, if given, revert. This is something from solidity compiler. It checks if some function 
// is payable, as none of them are, solidity knows that no call should have value, and if it does, it reverts.
// This is an optimization to save gas.
CALLVALUE           // [msg.value]
DUP1                // [msg.value, msg.value]
ISZERO              // [msg.value == 0, msg.value]
PUSH1 0x0e          // [0x0e, msg.value == 0, msg.value]
JUMPI               // [msg.value]  // If msg.value == 0, jump to 0x0e PC/JumpDest
PUSH0               // [0x00, msg.value]
DUP1                // [0x00, 0x00, msg.value]
REVERT              // [msg.value] If someone send value to this call, revert. Otherwise, jump

// JumpDest if msg.value == 0, start here!
// Also something from solidity compiler. It checks if the calldata size is less than 0x04, 
// the functon selector is 4 bytes. It does this because the compiler knows there is no 
// fallback function
JUMPDEST            // [msg.value]
POP                 // []           // Pop msg.value
PUSH1 0x04          // [0x04]
CALLDATASIZE        // [calldata_size, 0x04]
LT                  // [calldata_size < 0x04] boolean
PUSH1 0x30          // [0x30, calldata_size < 0x04]
JUMPI               // []          
// If calldata_size < 0x04, jump to 0x30 PC/JumpDest. Jump to "calldata_jump" 

// Function dispatch
PUSH0               // [0x00]
CALLDATALOAD        // [32 bytes of calldata]
PUSH1 0xe0          // [0xe0, 32 bytes of calldata]
SHR                 // [calldata[0:4]] function selector
DUP1                // [function selector, function selector]
PUSH4 0xcdfead2e    // [0xcdfead2e, function selector, function selector] // 0xcdfead2e = updateHorseNumber
EQ                  // [function selector == 0xcdfead2e, function selector]
PUSH1 0x34          // [0x34, function selector == 0xcdfead2e, function selector]
JUMPI               // [function selector]  // If function selector == 0xcdfead2e, jump to 0x34 PC/JumpDest

DUP1                // [function selector, function selector]
PUSH4 0xe026c017    // [0xe026c017, function selector, function selector] // 0xe026c017 = readNumberOfHorses
EQ                  // [function selector == 0xe026c017, function selector]
PUSH1 0x45          // [0x45, function selector == 0xe026c017, function selector]
JUMPI               // [function selector]  // If function selector == 0xe026c017, jump to 0x45 PC/JumpDest

// calldata_jump
// Revert jumpdest
JUMPDEST                // []
PUSH0                   // [0x00]
DUP1                    // [0x00, 0x00]
REVERT                  // []           // Revert if calldata is so small to be a function selector

// updateHorseNumber jumpdest 1
// setup jumping program counters in stack
JUMPDEST                // [function selector]
PUSH1 0x43              // [0x43, function selector]
PUSH1 0x3f              // [0x3f, 0x43, function selector]
CALLDATASIZE            // [calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x04              // [0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x59              // [0x59, 0x04, calldata_size, 0x3f, 0x43, function selector]
JUMP                    // [0x04, calldata_size, 0x3f, 0x43, function selector]


// updateHorseNumber jumpdest 4
// Update the horse number
JUMPDEST                // [calldata: of number to udate, 0x43, function selector]
PUSH0                   // [0x00, calldata: of number to udate, 0x43, function selector]
SSTORE                  // [0x43, function selector]
JUMP                    // [function selector]

// updateHorseNumber jumpdest 5
// end execution
JUMPDEST                // [function selector]
STOP                    // []

// readNumberOfHorses jumpdest 1
JUMPDEST                // [function selector]  
PUSH0                   // [0x00, function selector]
SLOAD                   // [number of horses, function selector]
PUSH1 0x40              // [0x40, number of horses, function selector]
MLOAD                   // [0x80, number of horses, function selector], Memory 0x40 -> 0x80 Load whatevere is at 0x40
SWAP1                   // [number of horses, 0x80, function selector]
DUP2                    // [0x80, number of horses, 0x80, function selector]
MSTORE                  // [0x80, function selector]   // Memory: [number of horses] at 0x80
PUSH1 0x20              // [0x20, 0x80, function selector] // Memory [number of horses] at 0x80
ADD                     // [0xa0, function selector] // Memory [number of horses] at 0x80
PUSH1 0x40              // [0x40, 0xa0, function selector] // Memory [number of horses] at 0x80
MLOAD                   // [0x80, 0xa0, function selector] // Memory [number of horses] at 0x80
DUP1                    // [0x80, 0x80, 0xa0, function selector]
SWAP2                   // [0xa0, 0x80, 0x80, function selector]
SUB                     // [0x80 - 0xa0, 0x80, function selector]
SWAP1                   // [0x80, 0x80 - 0xa0, function selector]
RETURN                  // [function selector] // Return a value of 32 bytes, thats located at position 0x80 in memory

// updateHorseNumber jumpdest 2
// Check if there is a value to update the horse number
JUMPDEST                // [0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH0                   // [0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x20              // [0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
DUP3                    // [0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]         
DUP5                    // [calldata_size, 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
SUB                     // [calldata_size - 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
// Is there more calldata than the function selector?
SLT                     // [calldata_size - 0x04 < 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
ISZERO                  // [calldata_size - 0x04 == 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x68              // [0x68, calldata_size - 0x04 == 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
JUMPI                   //  [0x00, 0x04, calldata_size, 0x3f, 0x43, function selector] 

// Revert if there is not enough calldata
PUSH0                   // [0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
DUP1                    // [0x00, 0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
REVERT                  // []

// updateHorseNumber jumpdest 3
// Grab the calldata to update the horse number
JUMPDEST                //  [0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
POP                     //  [0x04, calldata_size, 0x3f, 0x43, function selector]
CALLDATALOAD            // [calldata: of number to udate, calldata_size, 0x3f, 0x43, function selector]
SWAP2                   // [0x3f, calldata_size, calldata: of number to udate, 0x43, function selector]
SWAP1                   // [calldata_size, 0x3f, calldata: of number to udate, 0x43, function selector]
POP                     // [0x3f, calldata: of number to udate,  0x43, function selector]
JUMP                    // [calldata: of number to udate, 0x3f, 0x43, function selector]

// 3. Metadata. This is unnaccesuble code
// Solidity makes it to jnow about compiler version and optimization settings, things like that
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
INVALID
PUSH23 0xcfaa26a978c7cbd6472fe8f6998f4608429a6fb1e97f12
CALL
INVALID
REVERT
SIGNEXTEND
MSTORE
GT
SAR
PUSH5 0x736f6c6343
STOP
ADDMOD
EQ
STOP
CALLER