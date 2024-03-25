# Running the Stone Prover in a Gevulot node

This readme will take you through all the steps to build and run the Stone prover.





## 1 Gathers the Inputs

This process begin with the files generated from 

## 2 Processing the inputs


### 2.1  Upload the input files

The four input files must have a public url, in order for the Gevulot nodes to access them in a proof.  Often, an S3 bucket is used. It is important to make sure that the files have public read permission

### 2.2  Calculate the hashes

Each file input requires a hash value, which is a 256-bit value and 64-char hex string.  That may be computed by `gevulot-cli`.

```
./gevulot-cli calculate-hash --file private_input.json
```

## 3 Execute the proof

### 3.1  The configuration files


The `prover_config_file` and `parameter_file` parameters point to files embedded in the prover unikernel.  This is their contents



`cpu_air_params.json`

{
    "field": "PrimeField0",
    "stark": {
        "fri": {
            "fri_step_list": [
                0,
                4,
                3
            ],
            "last_layer_degree_bound": 64,
            "n_queries": 18,
            "proof_of_work_bits": 24
        },
        "log_n_cosets": 4
    },
    "use_extension_field": false
}

`cpu_air_prover_config.json`
{
    "cached_lde_config": {
        "store_full_lde": false,
        "use_fft_for_eval": false
    },
    "constraint_polynomial_task_size": 256,
    "n_out_of_memory_merkle_layers": 1,
    "table_prover_n_tasks_per_segment": 32
}

Of course, you may choose or require the use of different config files.  In that case, treat them like normal input files, and changes the parameters in the struct passed in to the prover.

### 3.2  The params stucture for the proof

Note that the program hashes are static, corresponding to the deployed prover and verifier

```
Prover hash:ae713ce2635872d7dfe8607a5ebd129164954276d7cab131977bdc24c02f573d
Verifier hash:140abc53852d49b3d940c1df462d5eec0fb6b55adaae49f8be07bce678378ac1.
```

```
[
    {
        "program": "ae713ce2635872d7dfe8607a5ebd129164954276d7cab131977bdc24c02f573d",
        "cmd_args": [
            {
                "name": "--out_file",
                "value": "/workspace/proof.json"
            },
            {
                "name": "--private_input_file",
                "value": "/workspace/fibonacci_private_input.json"
            },
            {
                "name": "--public_input_file",
                "value": "/workspace/fibonacci_public_input.json"
            },
            {
                "name": "--prover_config_file",
                "value": "/gevulot/cpu_air_prover_config.json"
            },
            {
                "name": "--parameter_file",
                "value": "/gevulot/cpu_air_params.json"
            }
        ],
        "inputs": [
            {
                "Input": {
                    "local_path": "2fa6e451e18f82959c58363b274586964c133c71d1fae5116027c0b5de245f8c",
                    "vm_path": "/workspace/fibonacci_memory.json",
                    "file_url": "https://gevulot.eu-central-1.linodeobjects.com/fibonacci_memory.json"
                }
            },
            {
                "Input": {
                    "local_path": "3a26731cba5db571f740345cb2803687b41b329e4f033ee8bc1a7a26c508fae9",
                    "vm_path": "/workspace/fibonacci_private_input.json",
                    "file_url": "https://gevulot.eu-central-1.linodeobjects.com/fibonacci_private_input.json"
                }
            },
            {
                "Input": {
                    "local_path": "82c352f2c1a5088c71aa87cb6c106c36b2eb56d207ef22d2d2c61a5e49abb521",
                    "vm_path": "/workspace/fibonacci_public_input.json",
                    "file_url": "https://gevulot.eu-central-1.linodeobjects.com/fibonacci_public_input.json"
                }
            },
            {
                "Input": {
                    "local_path": "b345e7ca8ab9bba28fe1438b3cabc8e2a9b6029c68663326be5b59097197903d",
                    "vm_path": "/workspace/fibonacci_trace.json",
                    "file_url": "https://gevulot.eu-central-1.linodeobjects.com/fibonacci_trace.json"
                }
            }
        ]
    },
    {
        "program": "140abc53852d49b3d940c1df462d5eec0fb6b55adaae49f8be07bce678378ac1",
        "cmd_args": [
            {
                "name": "--in_file",
                "value": "/workspace/proof.json"
            }
        ],
        "inputs": [
            {
                "Output": {
                    "source_program": "ae713ce2635872d7dfe8607a5ebd129164954276d7cab131977bdc24c02f573d",
                    "file_name": "/workspace/proof.json"
                }
            }
        ]
    }
]
```


### 3.3  Execute the proof

Prover hash:ae713ce2635872d7dfe8607a5ebd129164954276d7cab131977bdc24c02f573d
Verifier hash:140abc53852d49b3d940c1df462d5eec0fb6b55adaae49f8be07bce678378ac1.


## Verify the results

## 4.1 Print the transaction tree

## 4.2 Examine a leaf














```

## Execute

```
./target/release/gevulot-cli --jsonurl http://api.devnet.gevulot.com:9944  exec --tasks '[{"program":"ae713ce2635872d7dfe8607a5ebd129164954276d7cab131977bdc24c02f573d","cmd_args":[{"name":"--out_file","value":"/workspace/proof.json"},{"name":"--private_input_file","value":"/workspace/fibonacci_private_input.json"},{"name":"--public_input_file","value":"/workspace/fibonacci_public_input.json"},{"name":"--prover_config_file","value":"/gevulot/cpu_air_prover_config.json"},{"name":"--parameter_file","value":"/gevulot/cpu_air_params.json"}],"inputs":[{"Input":{"local_path":"2fa6e451e18f82959c58363b274586964c133c71d1fae5116027c0b5de245f8c","vm_path":"/workspace/fibonacci_memory.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_memory.json"}},{"Input":{"local_path":"3a26731cba5db571f740345cb2803687b41b329e4f033ee8bc1a7a26c508fae9","vm_path":"/workspace/fibonacci_private_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_private_input.json"}},{"Input":{"local_path":"82c352f2c1a5088c71aa87cb6c106c36b2eb56d207ef22d2d2c61a5e49abb521","vm_path":"/workspace/fibonacci_public_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_public_input.json"}},{"Input":{"local_path":"b345e7ca8ab9bba28fe1438b3cabc8e2a9b6029c68663326be5b59097197903d","vm_path":"/workspace/fibonacci_trace.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_trace.json"}}]},{"program":"140abc53852d49b3d940c1df462d5eec0fb6b55adaae49f8be07bce678378ac1","cmd_args":[{"name":"--in_file","value":"/workspace/proof.json"}],"inputs":[{"Output":{"source_program":"ae713ce2635872d7dfe8607a5ebd129164954276d7cab131977bdc24c02f573d","file_name":"/workspace/proof.json"}}]}]'
```

./target/release/gevulot-cli --jsonurl http://localhost:9944 get-tx-execution-output --hash 4b2f11462b9e882ef22859f3189b423604c0843f8af33342a82e368501d094a5


./target/release/gevulot-cli --jsonurl http://api.devnet.gevulot.com:9944 print-tx-tree 2c03326e4e64825a0bf4ac11dfb8c98ae5d7a2698805a7f31d6db660c987a9de

./target/release/gevulot-cli --jsonurl http://api.devnet.gevulot.com:9944 get-tx adc17040f92223326b3c9a25d994399895ebb1e360252e052ba63339b2155c79

./target/release/gevulot-cli --jsonurl http://api.devnet.gevulot.com:9944 get-tx adc17040f92223326b3c9a25d994399895ebb1e360252e052ba63339b2155c79 | jq -r '.payload.Verification.verification' | base64 -d

 | jq -r '.payload.Verification.verification' | base64 -d