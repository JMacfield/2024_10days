#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <Joint.h>
#include <Node.h>

struct Skeleton {
#pragma region メンバ関数

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="rootNode"></param>
	void Create(const Node& rootNode);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

#pragma endregion



	
	int32_t root_;
	
	std::map<std::string, int32_t>jointMap_;
	
	std::vector<Joint> joints_;


};