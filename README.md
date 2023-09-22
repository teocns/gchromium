# Selective Proxy

> 👋 [Active feature-QA thread](https://github.com/teocns/gchromium/issues/5)
>
> 🐛 [Issues Tracker](https://github.com/teocns/gchromium/issues?q=is%3Aopen+is%3Aissue+label%3Aselective-proxy)

The _"Selective Proxy"_ feature allows to proxy an intercepted request by passing the header `Stealthium-Proxy` to the requests' interception.

It is implemented on branch [selective-proxy](https://github.com/teocns/gchromium/tree/selective-proxy), and was first presented with this [Pull Request](https://github.com/teocns/gchromium/pull/3). 




#### Quick Start

```  
  proxy = 'http://user:password@host:port';

  await page.setRequestInterception(true);
  
  page.on('request', async (request) => {

    // Ensure that the request is a valid http scheme
    if (!request.url().startsWith('http')) {
      return request.continue();
    }
    
    // If this is a mime type that we don't want to proxy, continue
    const mime = request.resourceType();
    if (mime === 'image' || mime === 'media' || mime === 'font' || mime === 'stylesheet') {
      return request.continue();
    }
    
    console.warn(request.url())
    await request.continue({
      headers: {
        "Stealthium-Proxy": `https=${proxy};http=${proxy}`
      }
    });
  });  
```

The format follows that of the proxy parameter of [Chromium's network switches](https://www.chromium.org/developers/design-documents/network-settings/):


```--proxy-server=<scheme>=<uri>[:<port>][;...] | <uri>[:<port>] | "direct://"```

> ⚠️ Providing only `http=` (without `https=`) will cause _https_ requests to perform *without* a proxy!


#### Releases


##### MacOS ARM64
- s3://chromium-build-us-east-2/mac-builds/selective-proxy.tar


##### Linux X86_64
- s3://chromium-build-us-east-2/builds/31ac02d1-b4cf-4412-b6be-c4a7e2189f06/dist.tar.gz