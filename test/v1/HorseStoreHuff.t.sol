// SPDX-License-Identifier: MIT

pragma solidity 0.8.20;

import {Base_TestV1, IHorseStore} from "./Base_TestV1.t.sol";
import {HuffDeployer} from "foundry-huff/HuffDeployer.sol";

contract HorseStoreHuff is Base_TestV1 {
    // It assumes it is always on src folder. So the path is without src and without .huff extension
    string public constant horseStoreLocation = "horseStoreV1/HorseStore";

    function setUp() public override {
        // horseStore = HorseStore(
        //     HuffDeployer.config().deploy(HORSE_STORE_HUFF_LOCATION)
        // );
        horseStore = IHorseStore(
            HuffDeployer.config().deploy(horseStoreLocation)
        );
    }
}
