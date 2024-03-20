
## Build the ops images

```
ops build ./build/bazelbin/src/starkware/main/cpu/cpu_air_prover   -n -c ./gevulot/manifest_prover.json
ops build ./build/bazelbin/src/starkware/main/cpu/cpu_air_verifier -n -c ./gevulot/manifest_verifier.json
```

### Obtain hash strings

```
../gevulot/target/release/gevulot-cli calculate-hash --file ~/.ops/images/cpu_air_prover
   = 2572ff8676802455ac766f2d8a49fee5f98ef340d2fdcfbf52fa6ffb8b8cd0ab
../gevulot/target/release/gevulot-cli calculate-hash --file ~/.ops/images/cpu_air_verifier
   = d2330ccc4b749afc0895ec132539ba44f37054a36c60e4872ac589b246d56fa3
```

- Upload the files to linode
- Set read permissions to public


### Deploy

```
./target/release/gevulot-cli --jsonurl "http://localhost:9944" \
deploy \
--name "Stone prover & verifier" \
--prover 2572ff8676802455ac766f2d8a49fee5f98ef340d2fdcfbf52fa6ffb8b8cd0ab \
--proverimgurl 'https://gevulot.eu-central-1.linodeobjects.com/cpu_air_prover' \
--provercpus 4 \
--provermem 8192 \
--provergpus 0 \
--verifier d2330ccc4b749afc0895ec132539ba44f37054a36c60e4872ac589b246d56fa3 \
--verifierimgurl 'https://gevulot.eu-central-1.linodeobjects.com/cpu_air_verifier' \
--verifiercpus 4 \
--verifiermem 8192 \
--verifiergpus 0
```

Get back
```
Prover / Verifier deployed correctly.
Prover hash:5d6193b52033e81c1751dbd5c2d2aaa5ff5ae98ddbb536316d82b9bd73d3f9d6
Verifier hash:9ed5299890b1c5da313aad9e60d7705e5f709fd6bd7d982c010423b7745db7cc.
Tx Hash:b8d79fd13985ce4f2023c882fd1b93736ab2d2dd7440e768edc398f5443dcae2
```

## Try a proof

Let's copy the executable from the build directory

```
cp build/bazelbin/src/starkware/main/cpu/cpu_air_prover .
cp build/bazelbin/src/starkware/main/cpu/cpu_air_verifier .
```

Here is a sample proof

cpu_air_prover \
    --out_file=fibonacci_proof.json \
    --private_input_file=fibonacci_private_input.json \
    --public_input_file=fibonacci_public_input.json \
    --prover_config_file=cpu_air_prover_config.json \
    --parameter_file=cpu_air_params.json

And here, a sample verification