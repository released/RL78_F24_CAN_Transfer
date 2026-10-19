# RL78/F24 MCU CAN device Transfer

非破壞 Classical CAN500K benchmark，使用 Custom Transfer v1 格式：
standard request710/response711、8-byte request/reply、sequence／CRC8／echo／pattern／counter。
Tool 使用 CAN TAB 的 Custom Transfer，而不是 CAN Upgrade。
TargetF24B，短版 identity 只回8字元 part prefix；Boot/App INFO 才提供完整 part。

使用本專案自己的 SC 與相鄰 Boot 的 `common`，4MHz X1、P10/P11 CAN、P12 STB LOW，
診斷 UART P15/P16 115200。
細節見 [F24 device 工程指南](../RL78_F24_Boot_loader_CAN/README.md)。
Transfer 是獨立整機映像，會取代 Boot；不是 Boot 上的 App，不可當更新 BIN。
先以燒錄器測 Transfer；確認電氣層後再換裝 Boot + App。
正常 discovery／Transfer Test 的 Tool 與 MCU UART 紀錄見
[Custom Transfer 操作範例](https://released.github.io/slide_isp_tool_for_RL78/#f24-custom-transfer-example)。
CAN 波形量測、故障注入與恢復另行驗證。

```powershell
..\RL78_F24_Boot_loader_CAN\build.ps1 -Kind transfer
```

## UART 與錯誤統計

啟動時列出 clock 設定、CAN 初始化 result／stage、global／channel status、
request／response／filter 及實際 P12 output latch。
`STB=P12_LOW` 表示 MCU 設定的輸出值，不是 transceiver 供電或電壓的量測。

主迴圈擷取 Classical CAN 錯誤旗標，更新 GET_STATS 的 warning／passive／bus-off
計數及 last flags；UART 錯誤摘要最多每 250 ms 輸出一次。
硬體旗標會合併重複事件，counter 表示擷取次數，並非每一次實體錯誤的精確總數。
啟動欄位、stage 與判讀方式集中於 [Custom Transfer 診斷](https://released.github.io/slide_isp_tool_for_RL78/#transfer-diagnostics)。

`CAN_TRANSFER_UART_TRACE_ENABLE` 預設為 `1U`。
量測吞吐量時設為 `0U` 後重建，以排除 115200-baud 封包／錯誤文字輸出的影響；
錯誤擷取與 GET_STATS 計數仍保留。

維護者可使用既有 MSVC 開發環境執行離線 register／protocol 模型；一般韌體建置不需要 MSVC：

```powershell
..\RL78_F24_Boot_loader_CAN\test.ps1 -TransferModels
```

模型涵蓋初始化階段、旗標清除、統計與 timer wrap；實板 CAN／ACK、故障注入與恢復另行驗證。
