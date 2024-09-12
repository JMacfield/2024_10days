#include "Player.h"

Player* Player::instance = nullptr;

Player* Player::GetInstance()
{
	if(instance==nullptr){
		instance = new Player();
	}
	return instance;
}

Player::Player() {}

Player::~Player() {}

void Player::Init() {

	// モデル設定
	modelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/player", "player.gltf");
	model_.reset(Model::Create(modelHandle_));

	// ワールド座標 初期化
	worldTransform_.Initialize();
	worldTransform_.translate_.y = 4000.0f + 1000.0f;

	// 移動量 初期化
	vel_ = { 0.0f,-kGravityPower_,0.0f };
	// 加速度 初期化
	acc = 0.0f;

	// 線形補間時間(0.0f ~ 1.0f)
	normalT = 0.0f;
	// イージング時間(0.0f ~ 1.0f)
	easeT = 0.0f;
	// 衝突時の減算量を保持(0.0f ~ 1.0f)
	reverseT = 0.0f;

	// ゲーム時間(単位は秒)
	inGameTime = 70.0f;


}

void Player::Update() {

	// time加算処理
	if (normalT < 1.0f) {

		// 被弾時に保持した値が0より上であればtを減少させる
		if (reverseT > 0.0f) {
			reverseT -= 1.0f / (kFlamerate * inGameTime);
			normalT -= 1.5f / (kFlamerate * inGameTime);

			// 減速しすぎた場合、補正する
			if (normalT < 0.0f) {
				normalT = 0.0f;
				reverseT = 0.0f;
			}

			if (reverseT < 0.0f) {
				reverseT = 0.0f;
			}
		}
		// 通常時の処理 

		// フレームレート * ゲーム時間(3分)で補間
		normalT += 1.0f / (kFlamerate * inGameTime);

		// イージング
		easeT = GetSpeedForEaseInOutQuad(normalT);
	}


	// 落下
	if (worldTransform_.translate_.y > 0.0f) {
		// イージングで移動量を設定
		vel_.y = ((1.0f - easeT) * 0.0f + easeT * 10.0f) * -1.0f;

		//acc += 0.0001f;
		//vel_.y += (-acc);
		worldTransform_.translate_.y += (vel_.y * 0.2f);
	}
	else if (worldTransform_.translate_.y < 0.0f) {
		worldTransform_.translate_.y = 0.0f;
		acc = 0.0000f;
		vel_.y = 0;
	}


	SetCollision();

	// ワールド座標 更新
	worldTransform_.Update();

	Debug();

}

void Player::Draw(Camera camera) {

	//  描画
	model_->Draw(worldTransform_, camera);
}

void Player::Debug()
{

#ifdef _DEBUG

	// ImGuiを表示する
	ImGui::Begin("Player");

	// パラメータ
	if (ImGui::CollapsingHeader("Parameters")) {
		ImGui::DragFloat3("Pos", &this->worldTransform_.translate_.x, 0.0f);
		ImGui::DragFloat3("Vel", &this->vel_.x, 0.0f);
		ImGui::DragFloat("Acc", &this->acc, 0.0f);

		
		std::ostringstream ossNormalT;// 小数点を2桁のみ表示
		ossNormalT << std::fixed << std::setprecision(2) << normalT;
		std::string strNormalT = "NormalT : " + ossNormalT.str();
		ImGui::ProgressBar(normalT, ImVec2(-1.0f, 0.0f), strNormalT.c_str());
		
		
		std::ostringstream ossEaseT;// 小数点を2桁のみ表示
		ossEaseT << std::fixed << std::setprecision(2) << easeT;
		std::string strEaseT = "EaseT : " + ossEaseT.str();
		ImGui::ProgressBar(easeT, ImVec2(-1.0f, 0.0f), strEaseT.c_str());

		const int32_t LineSize = 100;
		float values[LineSize] = {};
		float t = 0.0f;
		for (int32_t i = 0; i < LineSize; i++) {
			t = (float)i / (float)LineSize;
			t = GetSpeedForEaseInOutQuad(t);
			// イージングで移動量を設定
			values[i] = (1.0f - t) * 0.0f + t * 100.0f;
		}
		int valuesCount = sizeof(values) / sizeof(float);


		// 描画リストを取得
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		// グラフの描画位置を取得
		ImVec2 plotPos = ImGui::GetCursorScreenPos();
		// 折れ線グラフの描画
		ImGui::PlotLines("Speed meter", values, valuesCount,
			0, nullptr, 0.0f, 100.0f, ImVec2(200, 200));

		// プロットの幅を計算 (各点のX軸の間隔)
		float stepX = 200.0f / (valuesCount - 1);  // データ点間のX軸距離
		float easedValue = GetSpeedForEaseInOutQuad(normalT) * 100.0f;  // イージングに基づいた値

		// 各データポイントの画面座標を計算
		ImVec2 pointPos = ImVec2(plotPos.x + (valuesCount - 1) * stepX * normalT,
			plotPos.y + (1.0f - easedValue / 100.0f) * 200.0f);

		// 赤い円を描画
		draw_list->AddCircleFilled(pointPos, 3.0f, IM_COL32(255, 0, 0, 255)); // 赤い円
		// 円の座標が最大値の何％に相当するかを計算
		float percentX = ((pointPos.x - plotPos.x) / 200.0f);
		float percentY = ((plotPos.y + 200.0f - pointPos.y) / 200.0f);

		// 計算したパーセンテージを表示
		ImGui::Text("Circle Position (X, Y):");
		ImGui::Text("X: %.2f", percentX);
		ImGui::Text("Y: %.2f", percentY);
	}

	// デバッグ操作
	if (ImGui::CollapsingHeader("Advanced Setings")) {
		// リセット
		if (ImGui::Button("Reset")) { this->Init(); }
		// 被弾による加速度減少
		if (ImGui::Button("Slow")) { this->ResiveSpeedDoun(0.02f); }

	}

	ImGui::End();


#endif // _DEBUG


}

void Player::Final()
{
	delete instance;
	instance = nullptr;

}

void Player::Move(Vector3 direction) {

	// 移動量に応じて移動
	worldTransform_.translate_.x += direction.x;
	worldTransform_.translate_.z += direction.z;

	// 限界まで移動していたら押し戻す
	if (worldTransform_.translate_.x < -kLimitArea_.x) { worldTransform_.translate_.x = -kLimitArea_.x; }
	if (worldTransform_.translate_.x > kLimitArea_.x) { worldTransform_.translate_.x = kLimitArea_.x; }
	if (worldTransform_.translate_.z < -kLimitArea_.y) { worldTransform_.translate_.z = -kLimitArea_.y; }
	if (worldTransform_.translate_.z > kLimitArea_.y) { worldTransform_.translate_.z = kLimitArea_.y; }


}


void Player::ResiveSpeedDoun(float power) {

	// powerに応じてtの値をを減少させる
	this->reverseT += power;

}

float Player::GetSpeedForEaseInOutQuad(float t)
{

	float result = 0.0f;
	if (t < 0.5f) {
		result = 2.0f * t * t;
	}
	else {
		result = 1 - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

	// tが負の値だったら逆方向にする
	if (t < 0.0f) { result *= -1.0f; }

	return result;
}

void Player::SetCollision() {
	collision_.center = worldTransform_.GetWorldPosition();
	GetOrientations(MakeRotateXYZMatrix(worldTransform_.rotate_.x, worldTransform_.rotate_.y, worldTransform_.rotate_.z), collision_.orientation);
	collision_.size = { 1.0f,1.0f,1.0f };
}