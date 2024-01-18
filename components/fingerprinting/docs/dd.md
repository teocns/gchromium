
# Device Descriptors storage


We are storing collected device descriptor on S3 into the `fingerprints` bucket.



```
aws s3 ls s3://fingerprints/
2023-12-14 09:00:26     118395 Chrome_100.0_Linux_Android_8.1.0_Moto_G_(4_Unknown_Device_df70c63e.json
2023-12-14 09:00:26     126345 Chrome_102.0_Linux_Android_12_moto_g_pure_Unknown_Device_ecb5e7a8.json
```



You can download all the DDs using


```
aws s3 cp s3://fingerprints/* <your_path>
```
