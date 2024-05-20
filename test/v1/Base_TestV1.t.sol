// SPDX-License-Identifier: MIT

pragma solidity 0.8.20;

import {HorseStore} from "../../src/horseStoreV1/HorseStore.sol";
import {IHorseStore} from "../../src/horseStoreV1/IHorseStore.sol";
import {Test, console2} from "forge-std/Test.sol";

abstract contract Base_TestV1 is Test {
    IHorseStore public horseStore;

    // This is the complete bytecode from the one written in Yul
    // 603680600a5f395ff3fe5f803560e01c8063cdfead2e1460245763e026c01714601b5780fd5b80602091548152f35b50602436106033576004359055005b80fd
    // I have to look for the invalid opcode in the Yul code. it is the `fe` in the bytecode and copy everything after it
    // 5f803560e01c8063cdfead2e1460245763e026c01714601b5780fd5b80602091548152f35b50602436106033576004359055005b80fd
    // And this is the runtime code

    function setUp() public virtual {
        horseStore = IHorseStore(address(new HorseStore()));
    }

    function testReadValue() public view {
        uint256 initialValue = horseStore.readNumberOfHorses();
        assertEq(initialValue, 0);
    }

    function testWriteValue() public {
        uint256 numberOfHorses = 777;
        horseStore.updateHorseNumber(numberOfHorses);
        assertEq(horseStore.readNumberOfHorses(), numberOfHorses);
    }
}
