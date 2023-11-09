# NanoBreathControllerUSBDriver

[English Version](#README_en-US)

[中文版](#README_zh-CN)

[日本語版](#README_ja-JP)

# README_en-US

## 1. Introduction

This project is a MIDI breath controller host software. Although it was developed for [NanoBreathControllerFirmware](https://github.com/A-KRY/NanoBreathControllerFirmware), it can be used with any firmware that sends MIDI CC values in the range of $[0, 127]$.

This project utilizes the following tools:

- [CLion](https://www.jetbrains.com/clion/) - for code writing, building, and testing.
- [Qt](https://www.qt.io/) - for GUI and Linguist.
- [teVirtualMIDI SDK](https://www.tobias-erichsen.de/software/virtualmidi/virtualmidi-sdk.html) - for certain MIDI-related functionalities. Please note that the usage of teVirtualMIDI SDK is subject to licensing restrictions by the author Tobias Erichsen. Make sure to review the author's license agreement for compliance.

## 2. Project Deployment

1. Download and install [CLion](https://www.jetbrains.com/clion/), [Qt](https://www.qt.io/), and [teVirtualMIDI SDK](https://www.tobias-erichsen.de/software/virtualmidi/virtualmidi-sdk.html), and select appropriate licenses.
2. Clone the project to your local machine through the provided link.
3. Open CMakeLists.txt and set the paths for Vulkan and teVirtualMIDISDK as indicated in the comments at the beginning of the file.
4. Attempt to build the project and run it.

## 3. Usage

Upon launching the program, the parameters set during the previous run will be retained.

### 3.1 System Tray Icon

#### Switch Language

Right-click on the system tray icon and select the desired language from the pop-up menu.

#### Exit the Program

Right-click on the system tray icon and select **Exit**. Please note that this is the only proper way to close the program.

### 3.2 Main Window

#### USB Port

Specifically, it refers to the serial communication port. The program periodically scans the serial ports and adds available serial communication devices to the dropdown menu. When you select a device, the program will close the connection to the current device and attempt to connect to the new one. If the connection fails, a dialog will pop up, and it will switch to **(Not Connected)**.

#### MIDI Channel

The target MIDI channel for MIDI signals, with 16 channels available (from 1 to 16).

#### CC (Continuous Controller)

The target CC channel for MIDI signals. Except for Pitch Bend, all CC channels supported in Ableton Live can be found here.

#### Smoothness

Adjusts the stability of the output signal. The tiny variations in human breath are amplified by the pressure sensor and ADC, necessitating the use of smoothing to reduce jitter. A smaller value results in an output signal closer to the raw signal, while a larger value makes the output signal change slower, or in other words, more sluggish.

Specifically, the program uses an exponential moving average filter (EMA) to smooth the output signal. The controls related to smoothness adjust the alpha value of the EMA filter.

*In reality, due to the lack of amplification circuitry in the author's current sensor module, the 10-bit resolution of the Arduino Nano's ADC is only utilized at about one-tenth of its potential. In the grim undersampling situation, the author had to set the alpha value of the EMA filter very small to mitigate the effects.*

## 4. Modifications

### Adding a Language

1. Choose an appropriate file name, recommended format is **{language}_{locale code}**, for example, "fr_FR.ts".
2. Open CMakeLists.txt and add "Resource/Translations/fr_FR.ts" under set(GEN_TS_FILES) (approximately around line 23).
3. Comment out the line set (TS_FILES ${GEN_TS_FILES}) (approximately around line 37) and reload the CMake project.
4. In CLion, select **generate_ts** in the **Run/Debug Configuration**, and click **Build**.
5. Uncomment set (TS_FILES ${GEN_TS_FILES}), and reload the CMake project.
6. In Qt Creator, open the project, find **Resource/resource.qrc**, right-click, and select "Add Existing Files" to add "fr_FR.ts".
7. Open Qt Linguist, open "fr_FR.ts," and update the translation.
8. Return to CLion, open CMakeLists.txt, add "fr_FR.qm" under set(GEN_TS_FILES) and reload the CMake project. In the **Run/Debug Configuration**, select **generate_qm**, and click **Build**.
9. Open **Src/main.cpp**, search for the comment **// Add a new language here**, and add the macro ADD_LANG. The first parameter is the file name of .qm, and the second parameter is the text to be displayed in the system tray icon's right-click menu, such as "French (Français)".


```C++
ADD_LANG(fr_FR, Français)
```

10.    Build and run the project using the **NanoBreathControllerUSBDriver** build option to verify the translation results.

## 5. License

This project is licensed under the GNU General Public License (GPL) version 3. For detailed information, please refer to the [LICENSE](LICENSE) file.

## 6. Contact

If you have any questions or feedback, you can contact me via [email](mailto:aliothkry@proton.me).

<center><i>Translation generated by ChatGPT.</i></center>



# README_zh-CN

## 1. 简介

本项目是 MIDI 呼吸控制器上位机驱动。虽然是为 [NanoBreathControllerFirmware](https://github.com/A-KRY/NanoBreathControllerFirmware) 编写的，但只要您的下位机发送的是 $[0, 127]$ 的 MIDI CC 值，均可使用本程序。

本项目使用了如下工具：

* [CLion](https://www.jetbrains.com/clion/) - 编写、构建、测试代码。
* [Qt](https://www.qt.io) - 使用 GUI 和 Linguist。
* [teVirtualMIDI SDK](https://www.tobias-erichsen.de/software/virtualmidi/virtualmidi-sdk.html) - 用于某些 MIDI 相关功能。请注意，teVirtualMIDI SDK 的使用受到作者 Tobias Erichsen 的许可要求限制，不得在未经作者事先批准的情况下分发该SDK。请查看作者的许可协议以确保合规性。

## 2. 部署项目

1. 下载安装 [CLion](https://www.jetbrains.com/clion/)、[Qt](https://www.qt.io)、[teVirtualMIDI SDK](https://www.tobias-erichsen.de/software/virtualmidi/virtualmidi-sdk.html) 并选择合适的许可。
2. 通过链接克隆项目到本地。
3. 打开 CMakeLists.txt，根据前几行的注释设置您的 Vulkan 和 teVirtualMIDISDK的路径。
4. 尝试构建项目并运行。

## 3. 使用

当启动程序时，上次运行时设置的参数均会保留。

### 3.1 系统托盘图标

#### 切换语言

鼠标右键单击系统托盘图标，在弹出菜单中选择目标语言。

#### 退出程序

鼠标右键单击系统托盘图标，在弹出菜单中选择**退出**。请注意，**这是唯一正常关闭本程序的方法**。

### 3.2 主窗口

#### USB 端口

准确来说是串行通信端口。程序会定时扫描串口，将可用串行通信设备加入下拉菜单中。当您选择某一设备时，程序会关闭与当前设备的连接，并尝试连接新设备。若连接失败，会弹窗提醒，并切换到<b>（未连接)</b>。

#### MIDI 通道

MIDI 信号的目标 MIDI 通道，有 1 到 16 共 16 个通道可选。

#### CC (Continuous Controller)

MIDI 信号的目标 CC 通道。除了 Pitch Bend 外，Ableton Live 中支持的均可在这里找到（毕竟用呼吸控制器来控制弯音轮可不是什么好点子）。

#### 平滑度

调整输出信号的稳定程度。人吹气的微小变化会被气压传感器和 ADC 放大，需要通过平滑滤波来减小抖动。平滑度越小，输出信号越接近原始信号；平滑度越大，输出信号变化越慢，或者说越迟钝。

准确来说，程序使用了指数移动平均滤波器 (Exponential Moving Average Filter) 来平滑输出信号。Smoothness 相关控件控制的是 EMA 滤波器的 alpha 值。

*实际上，由于作者目前的传感器模块缺乏放大电路，Arduino Nano 的  ADC 的 10 bit 分辨率可能才发挥了十分之一左右。在惨淡的欠采样状况下，作者不得不拉大平滑度进而将 EMA 滤波器的 alpha 值设置得非常小来缓解。*

## 4. 修改

### 添加语言

1.  选择规范的文件名，推荐使用 **{语言}_{地区码}** 的形式，例如 "fr_FR.ts"。

2. 打开 CMakeLists.txt ，在 set(GEN_TS_FILES) 下添加 "Resource/Translations/fr_FR.ts" （大约第 23 行的位置）。
3. 注释掉 set (TS_FILES ${GEN_TS_FILES}) 一行（大约第 37 行的位置），重新加载 CMake 项目。
4. 在 CLion 的**运行/调试配置**中选择**generate_ts**，点击**构建**。
5. 取消注释 set (TS_FILES ${GEN_TS_FILES})，重新加载 CMake 项目。
6. 打开 Qt Creator，打开项目，找到 **Resource/resource.qrc**，右键“添加现有文件”，添加"fr_FR.ts"。
7. 打开 Qt Linguist，打开 "fr_FR.ts"，更新翻译。
8. 回到 CLion，打开 CMakeLists.txt，在 set(GEN_TS_FILES) 下添加如 "fr_FR.qm"，重新加载 CMake 项目。在 **运行/调试配置**中选择**generate_qm**，点击**构建**。
9. 打开 **Src/main.cpp**，搜索注释 **// Add a new language here**，添加宏 ADD_LANG 即可。第一个参数为 .qm 的文件名，第二个参数为显示在系统托盘图标右键菜单中的字符，如

```C++
ADD_LANG(fr_FR, Français)
```

10. 使用 **NanoBreathControllerUSBDriver** 构建选项构建项目并运行，验证翻译成果。

## 5. 许可协议

本项目使用 GNU General Public License (GPL) 版本 3 许可。有关详细信息，请查看 [LICENSE](LICENSE) 文件。

## 6. 联系作者

如果您有任何疑问或反馈，可发送[邮件](mailto:aliothkry@proton.me)。



# README_ja-JP

##  1. 概要

このプロジェクトは、MIDIブレスコントローラホストソフトウェアです。[NanoBreathControllerFirmware](https://github.com/A-KRY/NanoBreathControllerFirmware)向けに開発されましたが、下位機器が$[0, 127]$のMIDI CC値を送信する限り、このプログラムを使用できます。

このプロジェクトでは以下のツールを使用しています：

- [CLion](https://www.jetbrains.com/clion/) - コードの記述、ビルド、テストに使用。
- [Qt](https://www.qt.io/) - GUIおよびLinguistに使用。
- [teVirtualMIDI SDK](https://www.tobias-erichsen.de/software/virtualmidi/virtualmidi-sdk.html) - 特定のMIDI関連機能に使用。teVirtualMIDI SDKの使用については、作者Tobias Erichsenのライセンス要件が適用され、SDKを作者の事前承認なしに配布することはできません。適合性を確認するために、作者のライセンス契約を確認してください。

## 2. プロジェクトのデプロイ

1. [CLion](https://www.jetbrains.com/clion/)、[Qt](https://www.qt.io/)、[teVirtualMIDI SDK](https://www.tobias-erichsen.de/software/virtualmidi/virtualmidi-sdk.html)をダウンロードしてインストールし、適切なライセンスを選択してください。
2. 提供されたリンクを使用してプロジェクトをクローンします。
3. CMakeLists.txtを開き、ファイルの先頭にあるコメントに従ってVulkanおよびteVirtualMIDISDKのパスを設定してください。
4. プロジェクトをビルドして実行してみてください。

## 3. 使用方法

プログラムを起動すると、前回の実行時に設定したパラメータが保持されます。

### 3.1 システムトレイアイコン

#### 言語切り替え

システムトレイアイコンを右クリックし、ポップアップメニューから目標の言語を選択してください。

#### プログラムの終了

システムトレイアイコンを右クリックし、**終了**を選択してください。プログラムを正常に終了する唯一の方法です。

### 3.2 メインウィンドウ

#### USBポート

正確にはシリアル通信ポートです。プログラムは定期的にシリアルポートをスキャンし、利用可能なシリアル通信デバイスをドロップダウンメニューに追加します。デバイスを選択すると、プログラムは現在のデバイスとの接続を閉じ、新しいデバイスへの接続を試みます。接続に失敗すると、ダイアログが表示され、<b>（未接続）</b>に切り替わります。

#### MIDIチャンネル

MIDI信号の対象MIDIチャンネルで、1から16までの16個のチャンネルが利用可能です。

#### CC（Continuous Controller）

MIDI信号の対象CCチャンネルです。Pitch Bendを除く、Ableton LiveでサポートされているすべてのCCチャンネルがここで見つけることができます。

#### スムージング

出力信号の安定度を調整します。人間の吹く微小な変化は、圧力センサーとADCによって増幅されるため、ジッタを減少させるためにスムージングが必要です。値を小さくすると、出力信号は生の信号に近づき、値を大きくすると、出力信号の変化が遅くなり、または鈍くなります。

具体的には、プログラムは出力信号をスムージングするために指数移動平均フィルタ（EMA）を使用しています。スムージングに関連するコントロールはEMAフィルタのアルファ値を調整します。

*実際には、作者の現在のセンサーモジュールには増幅回路がないため、Arduino NanoのADCの10ビット分解能はおおよそ1/10程度しか活用されていません。不十分なサンプリングの状況で、EMAフィルタのアルファ値を非常に小さく設定して影響を緩和する必要がありました。*

## 4. 変更

### 言語の追加

1. 適切なファイル名を選択し、<b>{言語}_{地域コード}</b>の形式を推奨します。たとえば、"fr_FR.ts"です。
2. CMakeLists.txtを開き、set(GEN_TS_FILES)の下に "Resource/Translations/fr_FR.ts"を追加してください（おおよそ23行目の位置）。
3. set (TS_FILES ${GEN_TS_FILES})の行をコメントアウトして（おおよそ37行目の位置）、CMakeプロジェクトを再読み込んでください。
4. CLionで**generate_ts**を選択し、**ビルド**をクリックしてください。
5. set (TS_FILES ${GEN_TS_FILES})のコメントを外し、CMakeプロジェクトを再読み込んでください。
6. Qt Creatorを開き、プロジェクトを開き、**Resource/resource.qrc**を見つけて右クリックし、既存のファイルを追加し、"fr_FR.ts"を追加してください。
7. Qt Linguistを開き、"fr_FR.ts"を開き、翻訳を更新してください。
8. CLionに戻り、CMakeLists.txtを開き、set(GEN_TS_FILES)の下に "fr_FR.qm"などを追加し、CMakeプロジェクトを再読み込んでください。**Run/Debug Configuration**で**generate_qm**を選択し、**ビルド**をクリックしてください。
9. **Src/main.cpp**を開き、コメント **// Add a new language here**を検索し、ADD_LANGマクロを追加してください。最初のパラメータは.qmファイルの名前で、2番目のパラメータはシステムトレイアイコンの右クリックメニューに表示されるテキストです。以下のように指定できます。

```C++
ADD_LANG(fr_FR, Français)
```

10. **NanoBreathControllerUSBDriver**のビルドオプションを使用してプロジェクトをビルドして実行し、翻訳結果を確認してください。

## 5. ライセンス

このプロジェクトはGNU General Public License（GPL）バージョン3のライセンスの下で提供されています。詳細については、[LICENSE](LICENSE)ファイルを参照してください。

## 6. 作者への連絡

質問やフィードバックがある場合、[メール](mailto:aliothkry@proton.me)をお送りいただけます。

<center><i>翻訳はChatGPTによって生成されました。</i></center>
