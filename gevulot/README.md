
## Build the ops images



build --define=KMP_USE_SHM=1

omg run



bazel build --define=KMP_USE_SHM=1 //...

```
ops build ./build/bazelbin/src/starkware/main/cpu/cpu_air_prover   -n -c ./gevulot/manifest_prover.json -i stone_prover && \
ops build ./build/bazelbin/src/starkware/main/cpu/cpu_air_verifier -n -c ./gevulot/manifest_verifier.json -i stone_verifier && \
s3cmd put --acl-public --force /home/ader/.ops/images/stone_prover s3://gevulot/images/stone_prover && \
s3cmd put --acl-public --force /home/ader/.ops/images/stone_verifier s3://gevulot/images/stone_verifier && \
../gevulot/target/release/gevulot-cli calculate-hash --file ~/.ops/images/stone_prover && \
../gevulot/target/release/gevulot-cli calculate-hash --file ~/.ops/images/stone_verifier



The hash of the file is: d4f18cfb4a67892792fb9c927abfe290a3b902f4141d6500e322c99d44c662e7
The hash of the file is: ca05979dd427e46ecdb1bf4e4368a4cbf400ff6531cc1fd0005832b457eaf9d1

https://gevulot.eu-central-1.linodeobjects.com/images/stone_prover
https://gevulot.eu-central-1.linodeobjects.com/images/stone_verifier


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


./target/release/gevulot-cli --jsonurl "http://localhost:9944" \
deploy \
--name "New Stone prover & verifier" \
--prover 450c9005e8d110351363011287449dddb0cdd4adcd24023f10d0ca066e5de9f4 \
--proverimgurl 'https://gevulot.eu-central-1.linodeobjects.com/images/stone_prover' \
--provername "new-stone" \
--provercpus 8 \
--provermem 8192 \
--provergpus 0 \
--verifier a35c38958aaf5dae86d789baac7c816ad09e057445d3d51e4cb7f3ab145042a9 \
--verifierimgurl 'https://gevulot.eu-central-1.linodeobjects.com/images/stone_verifier' \
--verifiername "new-stone" \
--verifiercpus 4 \
--verifiermem 8192 \
--verifiergpus 0



Get back
```
Prover / Verifier deployed correctly.
Prover hash:e5bc5a67f6c3382b775d8a50572ae456ff03c0b72bdc8095d7cf8d4e4ac24fde
Verifier hash:b3438918622ea9802e08b7f5716618708235c66cf9bccd12aa2c16cc99975785.
Tx Hash:ac0bfa0093344217e2e59da82f666e328d0925ca2d20fc529f5052c0a6d40522

## Execute

./target/release/gevulot-cli --jsonurl http://localhost:9944  exec --tasks '[{"program":"e5bc5a67f6c3382b775d8a50572ae456ff03c0b72bdc8095d7cf8d4e4ac24fde","cmd_args":[{"name":"--out_file","value":"/workspace/proof.json"},{"name":"--private_input_file","value":"/workspace/fibonacci_private_input.json"},{"name":"--public_input_file","value":"/workspace/fibonacci_public_input.json"},{"name":"--prover_config_file","value":"/gevulot/cpu_air_prover_config.json"},{"name":"--parameter_file","value":"/gevulot/cpu_air_params.json"}],"inputs":[{"Input":{"local_path":"2fa6e451e18f82959c58363b274586964c133c71d1fae5116027c0b5de245f8c","vm_path":"/workspace/fibonacci_memory.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_memory.json"}},{"Input":{"local_path":"3a26731cba5db571f740345cb2803687b41b329e4f033ee8bc1a7a26c508fae9","vm_path":"/workspace/fibonacci_private_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_private_input.json"}},{"Input":{"local_path":"82c352f2c1a5088c71aa87cb6c106c36b2eb56d207ef22d2d2c61a5e49abb521","vm_path":"/workspace/fibonacci_public_input.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_public_input.json"}},{"Input":{"local_path":"b345e7ca8ab9bba28fe1438b3cabc8e2a9b6029c68663326be5b59097197903d","vm_path":"/workspace/fibonacci_trace.json","file_url":"https://gevulot.eu-central-1.linodeobjects.com/fibonacci_trace.json"}}]},{"program":"b3438918622ea9802e08b7f5716618708235c66cf9bccd12aa2c16cc99975785","cmd_args":[{"name":"--in_file","value":"/workspace/proof.json"}],"inputs":[{"Output":{"source_program":"e5bc5a67f6c3382b775d8a50572ae456ff03c0b72bdc8095d7cf8d4e4ac24fde","file_name":"/workspace/proof.json"}}]}]'


./cpu_air_prover \
    --out_file=fibonacci_proof.json \
    --private_input_file=fibonacci_private_input.json \
    --public_input_file=fibonacci_public_input.json \
    --prover_config_file=cpu_air_prover_config.json \
    --parameter_file=cpu_air_params.json