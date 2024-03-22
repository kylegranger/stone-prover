
## Build the ops images

```
ops build ./build/bazelbin/src/starkware/main/cpu/cpu_air_prover   -n -c ./gevulot/manifest_prover.json
ops build ./build/bazelbin/src/starkware/main/cpu/cpu_air_verifier -n -c ./gevulot/manifest_verifier.json
```

### Obtain hash strings

```
../gevulot/target/release/gevulot-cli calculate-hash --file ~/.ops/images/cpu_air_prover
../gevulot/target/release/gevulot-cli calculate-hash --file ~/.ops/images/cpu_air_verifier
```

- Upload the files to public http server
- Set read permissions to public

### URLS & HASHES
https://gevulot.eu-central-1.linodeobjects.com/cpu_air_prover
  668c9bd6490b0bcdf1dea1b206c1417f8a00043d5a2902dbcc6eba7f6704305d

https://gevulot.eu-central-1.linodeobjects.com/cpu_air_verifier
  93de7295a06f31734e70912637f0ef0f058f537bba009d665ce11ea80ff9583d

https://gevulot.eu-central-1.linodeobjects.com/fibonacci_memory.json
  2fa6e451e18f82959c58363b274586964c133c71d1fae5116027c0b5de245f8c

https://gevulot.eu-central-1.linodeobjects.com/fibonacci_private_input.json
  3a26731cba5db571f740345cb2803687b41b329e4f033ee8bc1a7a26c508fae9
  
https://gevulot.eu-central-1.linodeobjects.com/fibonacci_public_input.json
  82c352f2c1a5088c71aa87cb6c106c36b2eb56d207ef22d2d2c61a5e49abb521

https://gevulot.eu-central-1.linodeobjects.com/fibonacci_trace.json
  b345e7ca8ab9bba28fe1438b3cabc8e2a9b6029c68663326be5b59097197903d

### Deploy

```
./target/release/gevulot-cli --jsonurl "http://localhost:9944" \
deploy \
--name "Stone prover & verifier" \
--prover 668c9bd6490b0bcdf1dea1b206c1417f8a00043d5a2902dbcc6eba7f6704305d \
--proverimgurl 'https://gevulot.eu-central-1.linodeobjects.com/cpu_air_prover' \
--provercpus 4 \
--provermem 8192 \
--provergpus 0 \
--verifier 93de7295a06f31734e70912637f0ef0f058f537bba009d665ce11ea80ff9583d \
--verifierimgurl 'https://gevulot.eu-central-1.linodeobjects.com/cpu_air_verifier' \
--verifiercpus 4 \
--verifiermem 8192 \
--verifiergpus 0
```

Get back
```
Prover / Verifier deployed correctly.
Prover hash:cba58e08d4ab4ccb5314a52740dfd943628491abf9d030602f5c301767fe2400
Verifier hash:3ac1195c735bc93eb807ed06ac1d36dac018ae8eb02c196164d5e590e2aa8d86.
Tx Hash:6c4c9a45ca85a2140c8abd597a85bcdf5b887222400529a25e0b65c1f6469f0d
```

## Execute

```
RUST_LOG=trace,gevulot=trace ./target/release/gevulot-cli --jsonurl http://localhost:9944  exec --tasks '[{"program":"cba58e08d4ab4ccb5314a52740dfd943628491abf9d030602f5c301767fe2400","cmd_args":[{"name":"--out_file","value":"/workspace/proof.json"},{"name":"--private_input_file","value":"/workspace/fibonacci_private_input.json"},{"name":"--public_input_file","value":"/workspace/fibonacci_public_input.json"},{"name":"--prover_config_file","value":"/gevulot/cpu_air_prover_config.json"},{"name":"--parameter_file","value":"/gevulot/cpu_air_params.json"}],"inputs":[{"Input":{"local_path":"2fa6e451e18f82959c58363b274586964c133c71d1fae5116027c0b5de245f8c","vm_path":"/workspace/fibonacci_memory.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_memory.json"}},{"Input":{"local_path":"3a26731cba5db571f740345cb2803687b41b329e4f033ee8bc1a7a26c508fae9","vm_path":"/workspace/fibonacci_private_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_private_input.json"}},{"Input":{"local_path":"82c352f2c1a5088c71aa87cb6c106c36b2eb56d207ef22d2d2c61a5e49abb521","vm_path":"/workspace/fibonacci_public_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_public_input.json"}},{"Input":{"local_path":"b345e7ca8ab9bba28fe1438b3cabc8e2a9b6029c68663326be5b59097197903d","vm_path":"/workspace/fibonacci_trace.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_trace.json"}}]},{"program":"3ac1195c735bc93eb807ed06ac1d36dac018ae8eb02c196164d5e590e2aa8d86","cmd_args":[{"name":"--in_file","value":"/workspace/proof.json"}],"inputs":[{"Output":{"source_program":"cba58e08d4ab4ccb5314a52740dfd943628491abf9d030602f5c301767fe2400","file_name":"/workspace/proof.json"}}]}]'
```