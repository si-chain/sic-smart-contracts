![SIC](logo.png)

--------------------

The sic-smart-contracts is EOSIO smart contract in the SIC system.

## Getting Started

*if you want to compile the code in you computer, you must have install the [EOSIO](https://github.com/EOSIO/eos), and get the eoscpp*

**compile** : `eoscpp -o target_file.wast source_file.cpp`

**get the abi** : `eoscpp -g target_file.abi source_file.hpp`

**set contract** : all the other action, you can find the Documents in [eos smart contracts](https://github.com/EOSIO/eos/wiki/Smart%20Contract)

## Smart Contract List

There are currently the following contracts：

| contract name | function | special account |
| :-: | :-: | :-: |
| sic.policy | commit ,review and upload the encrypted policy  | null |
| sic.token | The sic token to reward the user | 'sic.hole' to record the whole withdraw |
| sic.auth | Company to get the auth from user | null |
| license.list | To record the license that user agree | null |

To find out more about each contract, see the readme.md in each contract's directory

## NOTE:
Now the contract is tested in version Dawn 3.0. When the EOSIO Dawn 4.0 si stable, we will change the contract to Dawn 4.0.
