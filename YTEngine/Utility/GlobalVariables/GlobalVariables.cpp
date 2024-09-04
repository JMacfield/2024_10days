#include "GlobalVariables.h"
#include <ImGuiManager.h>
#include <imgui.h>

#include "WinApp.h"


GlobalVariables* GlobalVariables::GetInstance(){
    static GlobalVariables instance;
    return &instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName){
    //指定名のオブジェクトが無ければ追加
    datas_[groupName];
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value){
    //グループの参照
    Group& group = datas_[groupName];
    
    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;

    //設定した項目をstd::mapに追加
    group.items[key] = newItem;

}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value){
    //グループの参照
    Group& group = datas_[groupName];

    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;

    //設定した項目をstd::mapに追加
    group.items[key] = newItem;

}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3 value){
    //グループの参照
    Group& group = datas_[groupName];

    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;

    //設定した項目をstd::mapに追加
    group.items[key] = newItem;

}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value){
    

    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);


    //無かったら止める
    assert(itGroup != datas_.end());

    

    //for文で探そうと思ったがダメだった
    //一番上のようにfindでやった方が良いかも
    //まず参照
    Group& group = itGroup->second;
    std::map<std::string, Item>::iterator itItem = group.items.find(key);
    //無かったら追加
    if (itItem == itGroup->second.items.end()) {
        SetValue(groupName, key, value);


    }
    
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value){
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);


    //無かったら止める
    assert(itGroup != datas_.end());



    //for文で探そうと思ったがダメだった
    //一番上のようにfindでやった方が良いかも
    //まず参照
    Group& group = itGroup->second;
    std::map<std::string, Item>::iterator itItem = group.items.find(key);
    //無かったら追加
    if (itItem == itGroup->second.items.end()) {
        SetValue(groupName, key, value);


    }
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3 value){
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);


    //無かったら止める
    assert(itGroup != datas_.end());



    //for文で探そうと思ったがダメだった
    //一番上のようにfindでやった方が良いかも
    //まず参照
    Group& group = itGroup->second;
    std::map<std::string, Item>::iterator itItem = group.items.find(key);
    //無かったら追加
    if (itItem == itGroup->second.items.end()) {
        SetValue(groupName, key, value);


    }
}

int32_t GlobalVariables::GetIntValue(const std::string groupName, const std::string& key) {
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    assert(itGroup != datas_.end());

    Group& group = datas_.at(groupName);


    //キーを検索
    std::map<std::string, Item>::const_iterator itKey = group.items.find(key);

    assert(itKey != group.items.end());

    //SaveFileより
    return std::get<int32_t>(itKey->second.value);



}

float GlobalVariables::GetFloatValue(const std::string groupName, const std::string& key) {
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    assert(itGroup != datas_.end());

    Group& group = datas_.at(groupName);


    //キーを検索
    std::map<std::string, Item>::const_iterator itKey = group.items.find(key);

    assert(itKey != group.items.end());

    //SaveFileより
    return std::get<float>(itKey->second.value);

}

Vector3 GlobalVariables::GetVector3Value(const std::string groupName, const std::string& key) {
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    assert(itGroup != datas_.end());

    Group& group = datas_.at(groupName);


    //キーを検索
    std::map<std::string, Item>::const_iterator itKey = group.items.find(key);

    assert(itKey != group.items.end());

    //SaveFileより
    //普通にVector3で大丈夫だった
    return std::get<Vector3>(itKey->second.value);


}

void GlobalVariables::SaveFile(const std::string& groupName){
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup!=datas_.end());

    json root;
    //json::objectはstd::mapみたいなもの
    root = json::object();

    //jsonオブジェクト登録
    root[groupName] = json::object();

    //各項目について
    for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
        itItem != itGroup->second.items.end(); ++itItem) {
        //項目名を取得
        const std::string& itemName = itItem->first;
        //項目の参照を取得
        Item& item = itItem->second;


        //int32_tの場合
        if (std::holds_alternative<int32_t>(item.value)) {
            //int32_t型の値を登録
            root[groupName][itemName] = std::get<int32_t>(item.value);
        }

        //floatの場合
        else if (std::holds_alternative<float>(item.value)) {
            //float型の値を登録
            root[groupName][itemName] = std::get<float>(item.value);
        }

        //Vector3の場合
        else if (std::holds_alternative<Vector3>(item.value)) {
            //float型のjson配列登録
            Vector3 value = std::get<Vector3>(item.value);
            root[groupName][itemName] = json::array({value.x, value.y, value.z});
        }

    }


    //ディレクトリが無ければ作成する
    std::filesystem::path dir(DIRECTORY_PATH_);
    if (!std::filesystem::exists(DIRECTORY_PATH_)) {
        std::filesystem::create_directories(DIRECTORY_PATH_);
    }

    //書き込むJSONファイルのフルパスを合成する
    std::string filePath = DIRECTORY_PATH_ + groupName + ".json";
    //書き込む用ファイルストリーム
    std::ofstream ofs;
    //ファイルを書き込み用に開く
    ofs.open(filePath);

    //ファイルを開くのに失敗した場合
    if (ofs.fail()) {
        //書き込み用のファイルを開けるのに失敗したよ
        std::string message = "Failed open data file for write.";
        //WindowsAPIのメッセージボックスをここで使う
        MessageBoxA(nullptr, message.c_str(), "AdjustmentItems", 0);
        assert(0);
        return;
    }

    //ファイルにjson文字列を書き込む(インデント幅4)
    //rootオブジェクトに格納されている全データをJSON文字列に変換して、
    //書き込み用のファイルに流し込む。
    ofs << std::setw(4) << root << std::endl;


    //書き込み用に開いたファイルを閉じる。
    ofs.close();

    //これで終了！

}

void GlobalVariables::LoadFile(){
    //保存先ディレクトリのパスをローカル変数で宣言する
    std::filesystem::path dir(DIRECTORY_PATH_);

    //無かったらreturn
    if (!std::filesystem::exists(DIRECTORY_PATH_)) {
        return;
    }



    //各ファイルの処理
    std::filesystem::directory_iterator dir_it(DIRECTORY_PATH_);
    //directory_iterator...ディレクトリないのファイルを1つずつ指すためのポインタのようなもの
    for (const std::filesystem::directory_entry& entry : dir_it) {

        //パスによる判別
        //ファイルパスを取得
        const std::filesystem::path& filePath = entry.path();

        //ファイル拡張子を取得
        std::string extension = filePath.extension().string();
        //.jsonファイル以外はスキップ
        if (extension.compare(".json") != 0) {
            continue;
        }

        //extension...拡張子のみ抽出
        //stem...拡張子を除くファイル名を抽出


        //ファイル読み込み
        LoadFile(filePath.stem().string());

    }



}

void GlobalVariables::LoadFile(const std::string& groupName){
    //読み込むJSONファイルのフルパスを合成する
    std::string filePath = DIRECTORY_PATH_ + groupName + ".json";
    //読み込み用のファイルストリーム
    std::ifstream ifs;
    ifs.open(filePath);

    //書き込みはofstream
    //読み込みはifstream



    //読み込みが失敗した場合
    if (ifs.fail()) {
        //読み込み用のファイルを開けるのに失敗したよ
        std::string message = "Failed open data file for read.";
        //WindowsAPIのメッセージボックスをここで使う
        MessageBoxA(nullptr, message.c_str(), "AdjustmentItems", 0);
        assert(0);
        return;
    }

    json root;

    //json文字列からjsonのデータ構造に展開
    ifs >> root;
    //ファイルを閉じる
    ifs.close();


    //グループ登録確認
    //グループを検索
    json::iterator itGroup = root.find(groupName);

    //未登録チェック
    assert(itGroup != root.end());


    //確認アイテムについて
    for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
        //アイテム名(キー)を取得
        const std::string& itemName = itItem.key();
    
        if(itItem->is_number_integer()){
            //int型の値を登録
            int32_t value = itItem->get<int32_t>();
            SetValue(groupName, itemName, value);
        }
        else if (itItem->is_number_float()) {
            //float型の値を登録
            //floatは無いのでその代わりdoubleで
            double value = itItem->get<double>();
            SetValue(groupName, itemName, static_cast<float>(value));
        }
        //要素数3の配列であれば
        else if (itItem->is_array() && itItem->size() == 3) {
            //float型のison配列登録
            Vector3 value = {(itItem->at(0), itItem->at(1), itItem->at(2))};
            SetValue(groupName, itemName, value);
        }

    
    }

}

void GlobalVariables::Update(){
#ifdef _DEBUG

    //メニューバーを使用可能なフラグを付けてウィンドウを開く
    if (!ImGui::Begin("Global Variables",nullptr,ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    //このように条件を逆にしてからreturn ,continueを使うことを「早期リターン」と呼ぶ。
    //ネストが深くなるのを防げてコードが読みやすくなるよ。
    if (!ImGui::BeginMenuBar()) {
        return;
    }
    

    for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end(); ++itGroup) {

        //イテレータはペア一組分を指すポインタのようなもの
        //first...キー
        //second...値


        //グループ名を取得
        const std::string& groupName = itGroup->first;
        //グループの値を参照
        Group& group = itGroup->second;

        //グループメニューを追加する処理
        if (!ImGui::BeginMenu(groupName.c_str())) {
            continue;
        }


        //Group(map)の中にさらにItem(map)があるので二重for文だね
        //各項目について
        for (std::map<std::string, Item>::iterator itItem = group.items.begin(); 
            itItem != group.items.end(); ++itItem) {
            //項目名を取得
            const std::string& itItemName = itItem->first;
            //項目の参照を取得
            Item& item = itItem->second;

            //alternative...代替
            //「std::holds_alternative」で方の判別が出来るよ

            //int32_t型を持っている場合
            if (std::holds_alternative<int32_t>(item.value)) {
                //違いに注意！
                //「get」で値を取得
                //「get_if」でポインタを取得
                int32_t* ptr = std::get_if<int32_t>(&item.value);
                ImGui::SliderInt(itItemName.c_str(), ptr, 0, 100);
            }
            //float型を持っている場合
            else if (std::holds_alternative<float>(item.value)) {
                //ポインタの取得
                float* ptr = std::get_if<float>(&item.value);
                ImGui::SliderFloat(itItemName.c_str(), ptr, 0.0f, 100.0f);
            }
            //Vector3型を持っている場合
            else if (std::holds_alternative<Vector3>(item.value)) {
                //ポインタの取得
                Vector3* ptr = std::get_if<Vector3>(&item.value);
                //ここではVector3をfloatの配列ということにする
                ImGui::SliderFloat3(itItemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
            }



        }



        //セーブボタン
        ImGui::Text("\n");
        if (ImGui::Button("Save")) {
            SaveFile(groupName);
            std::string message = std::format("{}, json saved.", groupName);
            MessageBoxA(nullptr, message.c_str(), "AdjustmentItems", 0);
        }



        ImGui::EndMenu();

    }


    ImGui::EndMenuBar();
    ImGui::End();

#endif 
}
