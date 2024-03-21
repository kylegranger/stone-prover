
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
  c1896b85800242b67964dbfb9f2699628548f92d4eebe273903860a36d2125a2

https://gevulot.eu-central-1.linodeobjects.com/cpu_air_verifier
  93de7295a06f31734e70912637f0ef0f058f537bba009d665ce11ea80ff9583d

https://gevulot.eu-central-1.linodeobjects.com/fibonacci_memory.json
  2fa6e451e18f82959c58363b274586964c133c71d1fae5116027c0b5de245f8c

https://gevulot.eu-central-1.linodeobjects.com/fibonacci_private_input.json
  bf7908893a247315a1c3860fcf1bb9f2f8f49193267d0ef779e5b9acfd4d3972
  
https://gevulot.eu-central-1.linodeobjects.com/fibonacci_public_input.json
  82c352f2c1a5088c71aa87cb6c106c36b2eb56d207ef22d2d2c61a5e49abb521

https://gevulot.eu-central-1.linodeobjects.com/fibonacci_trace.json
  b345e7ca8ab9bba28fe1438b3cabc8e2a9b6029c68663326be5b59097197903d

### Deploy

```
./target/release/gevulot-cli --jsonurl "http://localhost:9944" \
deploy \
--name "Stone prover & verifier" \
--prover c1896b85800242b67964dbfb9f2699628548f92d4eebe273903860a36d2125a2 \
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
Prover hash:1ac2b2dc5a253cd08186760422a3dd331e6baf896d26eafc29453f4d304fce19
Verifier hash:3ac1195c735bc93eb807ed06ac1d36dac018ae8eb02c196164d5e590e2aa8d86.
Tx Hash:fb0683deed02e3fa2ad7649b4a77a50b91987bdb29e34703eeb21d21e02967d1

```

## Execute

```
RUST_LOG=trace,gevulot=trace ./target/release/gevulot-cli --jsonurl http://localhost:9944  exec --tasks '[{"program":"1ac2b2dc5a253cd08186760422a3dd331e6baf896d26eafc29453f4d304fce19","cmd_args":[{"name":"--out_file","value":"/workspace/proof.json"},{"name":"--private_input_file","value":"/workspace/fibonacci_private_input.json"},{"name":"--public_input_file","value":"/workspace/fibonacci_public_input.json"},{"name":"--prover_config_file","value":"/gevulot/cpu_air_prover_config.json"},{"name":"--parameter_file","value":"/gevulot/cpu_air_params.json"}],"inputs":[{"Input":{"local_path":"2fa6e451e18f82959c58363b274586964c133c71d1fae5116027c0b5de245f8c","vm_path":"/workspace/fibonacci_memory.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_memory.json"}},{"Input":{"local_path":"bf7908893a247315a1c3860fcf1bb9f2f8f49193267d0ef779e5b9acfd4d3972","vm_path":"/workspace/fibonacci_private_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_private_input.json"}},{"Input":{"local_path":"82c352f2c1a5088c71aa87cb6c106c36b2eb56d207ef22d2d2c61a5e49abb521","vm_path":"/workspace/fibonacci_public_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_public_input.json"}},{"Input":{"local_path":"b345e7ca8ab9bba28fe1438b3cabc8e2a9b6029c68663326be5b59097197903d","vm_path":"/workspace/fibonacci_trace.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_trace.json"}}]},{"program":"3ac1195c735bc93eb807ed06ac1d36dac018ae8eb02c196164d5e590e2aa8d86","cmd_args":[{"name":"--in_file","value":"/workspace/proof.json"}],"inputs":[{"Output":{"source_program":"1ac2b2dc5a253cd08186760422a3dd331e6baf896d26eafc29453f4d304fce19","file_name":"/workspace/proof.json"}}]}]'
```