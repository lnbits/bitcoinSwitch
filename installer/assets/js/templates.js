const stringifyNice = (data) => JSON.stringify(data, null, 2)
const templates = Object.freeze({
    elements: {
        fileName: "elements.json",
        value: stringifyNice([
            {
                "name": "password",
                "type": "ACInput",
                "value": "ToTheMoon1",
                "label": "Password for PoS AP WiFi",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "ssid",
                "type": "ACInput",
                "value": "",
                "label": "WiFi SSID",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "wifipassword",
                "type": "ACInput",
                "value": "",
                "label": "WiFi Password",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "socket",
                "type": "ACInput",
                "value": "",
                "label": "Copy link from LNURLDevices extension in LNbits",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "wallet",
                "type": "ACInput",
                "value": "",
                "label": "Optional (for threshold), wallet ID from LNbits",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "threshold",
                "type": "ACInput",
                "value": "",
                "label": "Optional (for threshold), amount to hit to trigger",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "thresholdPin",
                "type": "ACInput",
                "value": "",
                "label": "Optional (for threshold), pin to trigger",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "thresholdTime",
                "type": "ACInput",
                "value": "",
                "label": "Optional (for threshold), time to turn of for",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            }
        ])
    }
})

