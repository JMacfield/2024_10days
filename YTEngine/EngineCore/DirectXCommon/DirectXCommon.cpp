#include "DirectXCommon.h"

#include <thread>

DirectXCommon* DirectXCommon::GetInstance() 
{
	static DirectXCommon instance;
	return &instance;
}

ComPtr<ID3D12DescriptorHeap> DirectXCommon::GenarateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	ComPtr<ID3D12DescriptorHeap> descriptorHeap= nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};

	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	
	HRESULT hr = DirectXCommon::GetInstance()->m_device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	
	assert(SUCCEEDED(hr));
	
	return descriptorHeap;
}

ComPtr<ID3D12Resource> DirectXCommon::CreateBufferResource(size_t sizeInBytes) 
{
	ComPtr<ID3D12Resource> resource = nullptr;
	
	//頂点リソース用のヒープを設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	//バッファリソース。テクスチャの場合はまた別
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc_.Width = sizeInBytes;
	//バッファの場合はこれらは1に
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	//バッファの場合はこれに
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr;
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));

	return resource;
}

ComPtr<ID3D12Resource> DirectXCommon::CreateDepthStencilTextureResource(const int32_t width,const int32_t height) 
{
	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = width;
	//Textureの高さ
	resourceDesc.Height = height;
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行 or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//DepthStencilとして利用可能なフォーマット
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//サンプリングカウント。1固定
	resourceDesc.SampleDesc.Count = 1;
	//2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//DepthStencilとして使う通知
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	//VRAM上に作る
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	//1.0f(最大値)でクリア
	depthClearValue.DepthStencil.Depth = 1.0f;
	//フォーマット。Resourceと合わせる
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//Resourceの作成
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = DirectXCommon::GetInstance()->m_device_->CreateCommittedResource(
		&heapProperties,					//Heapの設定 
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定。特になし。
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値を書き込む状態にしておく
		&depthClearValue,					//Clear最適値
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	
	assert(SUCCEEDED(hr));

	return resource;
}

void DirectXCommon::GenerateDXGIFactory() 
{
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController;
	//ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif 
}

void DirectXCommon::SelectAdapter() 
{
	ComPtr<IDXGIFactory7> dxgiFactory;
	
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	
	assert(SUCCEEDED(hr));
	
	DirectXCommon::GetInstance()->m_dxgiFactory_ = dxgiFactory;

	ComPtr<IDXGIAdapter4> useAdapter = nullptr;

	//良い順でアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++) 
	{
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);

		assert(SUCCEEDED(hr));

		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) 
		{
			//採用したアダプタの情報をログに出力.(wstring)
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}

		//ソフトウェアアダプタだった場合無視
		useAdapter = nullptr;
	}

	assert(useAdapter != nullptr);
	
	DirectXCommon::GetInstance()->m_dxgiFactory_ = dxgiFactory;
	DirectXCommon::GetInstance()->m_useAdapter_ = useAdapter;
}

void DirectXCommon::GenerateD3D12Device() 
{
	//機能レベルとログ出力用の文字
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};

	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	
	for (size_t i = 0; i < _countof(featureLevels); ++i) 
	{
		HRESULT hr = {};
		hr = D3D12CreateDevice(
			DirectXCommon::GetInstance()->m_useAdapter_.Get(), 
			featureLevels[i], 
			IID_PPV_ARGS(&DirectXCommon::GetInstance()->m_device_));
		
		if (SUCCEEDED(hr)) 
		{
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}

	assert(DirectXCommon::GetInstance()->m_device_ != nullptr);
	
	Log("Complete create D3D12Device!!!\n");
}

void DirectXCommon::StopErrorWarning() 
{
#ifdef _DEBUG	
	ComPtr<ID3D12InfoQueue> infoQueue;

	if (SUCCEEDED(DirectXCommon::GetInstance()->m_device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) 
	{
		//ヤバいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		
		D3D12_MESSAGE_ID denyIds[] = 
		{
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		//抑制する
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};

		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}
#endif 
}

void DirectXCommon::GenerateCommand() 
{
	ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->m_device_;
	
	HRESULT hr = {};
	
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	
	hr = device->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue));
	
	assert(SUCCEEDED(hr));

	//コマンドアロケータを生成する
	ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	
	assert(SUCCEEDED(hr));

	//コマンドリストを生成する
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	hr = device->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		commandAllocator.Get(),
		nullptr, 
		IID_PPV_ARGS(&commandList));

	assert(SUCCEEDED(hr));

	DirectXCommon::GetInstance()->m_commandQueue_ = commandQueue;
	DirectXCommon::GetInstance()->m_commandAllocator_ = commandAllocator;
	DirectXCommon::GetInstance()->m_commandList_ = commandList;
	DirectXCommon::GetInstance()->m_device_ = device;
}

void DirectXCommon::GenerateSwapChain() 
{	
	SwapChain swapChain = {};
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.Width = WinApp::GetInstance()->GetClientWidth();		
	swapChainDesc.Height = WinApp::GetInstance()->GetClientHeight();	
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;					
	swapChainDesc.SampleDesc.Count = 1;								
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		
	swapChainDesc.BufferCount = 2;										
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			

	HRESULT hr = {};

	hr = DirectXCommon::GetInstance()->m_dxgiFactory_->CreateSwapChainForHwnd(
		DirectXCommon::GetInstance()->m_commandQueue_.Get(),
		hwnd, 
		&swapChainDesc, 
		nullptr, 
		nullptr, 
		reinterpret_cast<IDXGISwapChain1**>(&DirectXCommon::GetInstance()->swapChain));

	assert(SUCCEEDED(hr));
	
	DirectXCommon::GetInstance()->swapChain.swapChainDesc = swapChainDesc;
}

void DirectXCommon::MakeDescriptorHeap() 
{
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;
	
	rtvDescriptorHeap = GenarateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);

	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;

	depthStencilResource_ = CreateDepthStencilTextureResource( 
		WinApp::GetInstance()->GetClientWidth(),
		WinApp::GetInstance()->GetClientHeight());

	dsvDescriptorHeap_ = GenarateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	
	//Format 基本的にはResourceに合わせる
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	//2DTexture
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	
	//DSVHeapの先頭にDSVを作る
	DirectXCommon::GetInstance()->m_device_->CreateDepthStencilView(
		depthStencilResource_.Get(), 
		&dsvDesc, 
		dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());

	DirectXCommon::GetInstance()->m_rtvDescriptorHeap_ = rtvDescriptorHeap;
	DirectXCommon::GetInstance()->m_dsvDescriptorHeap_ = dsvDescriptorHeap_;
	DirectXCommon::GetInstance()->m_depthStencilResource_ = depthStencilResource_;
}

void DirectXCommon::PullResourcesFromSwapChain() 
{
	HRESULT hr = {};
	hr = DirectXCommon::GetInstance()->swapChain.m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(& DirectXCommon::GetInstance()->swapChain.m_pResource[0]));
	//上手く取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = DirectXCommon::GetInstance()->swapChain.m_pSwapChain->GetBuffer(1, IID_PPV_ARGS(& DirectXCommon::GetInstance()->swapChain.m_pResource[1]));
	assert(SUCCEEDED(hr));
}

void DirectXCommon::SetRTV() 
{
	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->m_device_;
	SwapChain swapChain = DirectXCommon::GetInstance()->swapChain;
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap =  DirectXCommon::GetInstance()->m_rtvDescriptorHeap_;

	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;			
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;
	//D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};
	rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2] = {};
	rtvHandles[0] = rtvStartHandle;
	
	device->CreateRenderTargetView(DirectXCommon::GetInstance()->swapChain.m_pResource[0].Get(), &rtvDesc, rtvHandles[0]);
	
	//２つ目のディスクリプタハンドルを得る(自力で)
	rtvHandles[1].ptr = rtvHandles[0].ptr + DirectXCommon::GetInstance()->m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	//２つ目を作る
	device->CreateRenderTargetView(DirectXCommon::GetInstance()->swapChain.m_pResource[1].Get(), &rtvDesc, rtvHandles[1]);

	DirectXCommon::GetInstance()->m_rtvDescriptorHeap_=rtvDescriptorHeap;

	HRESULT hr = {};
	uint64_t fenceValue = 0;
	ComPtr<ID3D12Fence> fence = nullptr;
	hr = DirectXCommon::GetInstance()->m_device_->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent = nullptr;
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	DirectXCommon::GetInstance()->rtvDesc_ = rtvDesc;
	DirectXCommon::GetInstance()->rtvStartHandle_=rtvStartHandle;
	DirectXCommon::GetInstance()->rtvHandles_[0] = rtvHandles[0];
	DirectXCommon::GetInstance()->rtvHandles_[1] = rtvHandles[1];
	DirectXCommon::GetInstance()->fenceValue_ = fenceValue;
	DirectXCommon::GetInstance()->fenceEvent_ = fenceEvent;
	DirectXCommon::GetInstance()->m_fence_ = fence;
}

void DirectXCommon::GenarateViewport() 
{
	D3D12_VIEWPORT viewport{};
	
	viewport.Width = float(WinApp::GetInstance()->GetClientWidth());
	viewport.Height = float(WinApp::GetInstance()->GetClientHeight());
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	
	DirectXCommon::GetInstance()->viewport_ = viewport;
}

void DirectXCommon::GenerateScissor() 
{
	D3D12_RECT scissorRect{};
	
	scissorRect.left = 0;
	scissorRect.right = WinApp::GetInstance()->GetClientWidth();
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::GetInstance()->GetClientHeight();

	DirectXCommon::GetInstance()->scissorRect_ = scissorRect;
}

void DirectXCommon::InitializeFPS() 
{
	DirectXCommon::GetInstance()->reference_ = std::chrono::steady_clock::now();
}


void DirectXCommon::Initialize() 
{
	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	InitializeFPS();

	//DXGIFactoryを生成
	GenerateDXGIFactory();
	//最適なアダプターを選択
	SelectAdapter();

	//デバイスを生成
	GenerateD3D12Device();

	////エラー・警告、即ち停止
	StopErrorWarning();

	////GPUに作業させよう
	//コマンドリストを作ろう
	GenerateCommand();

	//スワップチェーンを生成する
	GenerateSwapChain();

	MakeDescriptorHeap();

	PullResourcesFromSwapChain();

	SetRTV();

	//ビューポートの生成
	GenarateViewport();

	//シザーを生成
	GenerateScissor();
}



void DirectXCommon::UpdateFPS() 
{
	//1/60秒ピッタリの時間
	//1フレームの時間
	const std::chrono::microseconds MIN_TIME(uint64_t(1000000.0f / 60.0f));

	//1/60秒よりわずかに短い時間
	const std::chrono::microseconds MIN_CHECK_TIME(uint64_t(1000000.0f / 65.0f));

	//現在時間を取得する
	//VSync待ちが完了した時点での時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - DirectXCommon::GetInstance()->reference_);

	//前回から1/60秒経つまで待機する
	//1/60秒(よりわずかに短い時間)経っていない場合
	if (elapsed < MIN_CHECK_TIME) 
	{
		//1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - DirectXCommon::GetInstance()->reference_ < MIN_TIME) 
		{
			//1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	DirectXCommon::GetInstance()->reference_ = std::chrono::steady_clock::now();
}


void DirectXCommon::BeginFrame() 
{
	backBufferIndex_ = DirectXCommon::GetInstance()->swapChain.m_pSwapChain->GetCurrentBackBufferIndex();

	//TransitionBarrierの設定
	//今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	
	//Noneにする
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = DirectXCommon::GetInstance()->swapChain.m_pResource[backBufferIndex_].Get();
	
	//遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	
	//TransitionBarrierを張る
	DirectXCommon::GetInstance()->m_commandList_->ResourceBarrier(1, &barrier_);

	//描画先のRTVを設定する
	DirectXCommon::GetInstance()->m_commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, nullptr);
	
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };	//青っぽい色,RGBA
	
	DirectXCommon::GetInstance()->m_commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex_], clearColor, 0, nullptr);

	//描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DirectXCommon::GetInstance()->m_dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	DirectXCommon::GetInstance()->m_commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, &dsvHandle);
	DirectXCommon::GetInstance()->m_commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);	

	DirectXCommon::GetInstance()->m_commandList_->RSSetViewports(1, &viewport_);
	DirectXCommon::GetInstance()->m_commandList_->RSSetScissorRects(1, &scissorRect_);
}

void DirectXCommon::EndFrame() 
{
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	
	//TransitionBarrierを張る
	DirectXCommon::GetInstance()->m_commandList_->ResourceBarrier(1, &barrier_);

	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	HRESULT hr = {};
	hr = DirectXCommon::GetInstance()->m_commandList_->Close();
	assert(SUCCEEDED(hr));

	ID3D12CommandList* commandLists[] = { DirectXCommon::GetInstance()->m_commandList_.Get()};
	DirectXCommon::GetInstance()->m_commandQueue_->ExecuteCommandLists(1, commandLists);

	swapChain.m_pSwapChain->Present(1, 0);

	fenceValue_++;
	
	DirectXCommon::GetInstance()->m_commandQueue_->Signal(DirectXCommon::GetInstance()->m_fence_.Get(), fenceValue_);
	
	if (DirectXCommon::GetInstance()->m_fence_->GetCompletedValue() < fenceValue_) 
	{
		DirectXCommon::GetInstance()->m_fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
	
	UpdateFPS();

	hr = DirectXCommon::GetInstance()->m_commandAllocator_->Reset();
	assert(SUCCEEDED(hr));

	hr = DirectXCommon::GetInstance()->m_commandList_->Reset(DirectXCommon::GetInstance()->m_commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DirectXCommon::Release() 
{
	CloseHandle(fenceEvent_);
}


void DirectXCommon::CheckRelease() 
{
	
}