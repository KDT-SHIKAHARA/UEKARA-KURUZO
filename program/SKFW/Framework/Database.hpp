#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<typeindex>
#include<type_traits>
#include<functional>
#include<tuple>
#include<filesystem>
#include<stdexcept>
#include<mutex>


#include<cereal/archives/binary.hpp>
#include<cereal/types/string.hpp>
#include<cereal/types/vector.hpp>

#include<SKFW/Framework/Singleton.hpp>

template<typename T>
struct StructMeta;

//  csvに読み書きをするフィールドの設定用
#define REFLECT(TYPE,...)\
template<>\
struct StructMeta<TYPE>{\
    using Type = TYPE;\
    static constexpr auto fields = std::make_tuple(__VA_ARGS__); \
};

/// <summary>
/// csvからの読込とバイナリ変換と読込をする。
/// </summary>
class CSVLoader {
public:
    /// <summary>
    /// csvからデータを読み込む
    /// </summary>
    /// <typeparam name="T"> 読み込みたい構造体 </typeparam>
    /// <param name="filepath"> csvのフォルダパス </param>
    /// <param name="skipHeader"> ヘッダー行をスキップするかどうかのフラグ </param>
    /// <returns> 読み込んだ構造体のvector </returns>
    template<typename T>
    static std::vector<T> Load(const std::string& filepath, bool skipHeader = true) {
        //  ファイル展開
        std::ifstream file(filepath);
        //  読込失敗処理
        if (!file.is_open()) throw std::runtime_error("CSVファイルが開けませんでした。" + filepath);

        std::vector<T> result;  //  読み込んだ構造体のコレクションを保管
        std::string line;   //  csv1行のbuf
        bool skip = skipHeader; //  ヘッダー行を読み飛ばすかどうか

        //  読み込み処理
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (skip) { skip = false; continue; }    //  ヘッダー行アリなら1回だけスキップする。
            //  追加する際に1行単位で読み込んで変換したものを渡す。
            result.push_back(parseLine<T>(line));
        }

        return result;
    }

private:
    /// <summary>
    /// csvの1行を構造体に変換して返す
    /// </summary>
    /// <typeparam name="T"> 読み込みたい構造体 </typeparam>
    /// <param name="line"> csvの1行 </param>
    /// <returns> 読み込んだcsvの1行 </returns>
    template<typename T>
    static T parseLine(const std::string& line) {
        T obj{};    //  空のインスタンス生成

        std::stringstream ss(line);
        std::string cell;   //  １セル毎の文字の保管
        auto fields = StructMeta<T>::fields;    //  登録したcsvに読み込むフィールド (std::tuple -> std::pair<FieldPtr, "名前">);

        //  tupleを展開してラムダに渡す。
        //  例:(id, name, damage, cost) → ラムダ内で flds...
        std::apply([&](auto&&... flds) {
            //  tupleの各フィールドごとにcsvを読み込んで代入
            ((std::getline(ss, cell, ','), assignField(obj, flds.first, cell)), ...);
            }, fields); // apply

        return obj;

    } // parseLine


    /// <summary>
    /// 読み込んだ文字列を対応した型に変換する。
    /// </summary>
    /// <typeparam name="T"> 読み込みたい任意の構造体 </typeparam>
    /// <typeparam name="FieldPtr"> 各フィールドのポインタ型 </typeparam>
    /// <param name="obj"> 値を代入する構造体のインスタンス </param>
    /// <param name="field"> メンバポインタで指定するフィールド </param>
    /// <param name="value"> csvから読み込んだcell単位の文字 </param>
    template<typename T, typename FieldPtr>
    static void assignField(T& obj, FieldPtr field, const std::string& value) {
        //  decltypeは型の取得 decay_tは普通の型に変換する。
        using FieldType = std::decay_t<decltype(obj.*field)>;
        //  コンパイル時に型判定、該当しなければ削除。全てにあてはならなかったら例外
        if constexpr (std::is_same_v<FieldType, int>) obj.*field = std::stoi(value);
        else if constexpr (std::is_same_v<FieldType, float>) obj.*field = std::stof(value);
        else if constexpr (std::is_same_v<FieldType, double>)obj.*field = std::stod(value);
        else if constexpr (std::is_same_v <FieldType, std::string >) obj.*field = value;
        else static_assert(always_false<FieldType>::value, "変換できない値です。");
    }

    //  型そのものを使ってtrue:false (c++ templateくらい)
    template<typename>
    struct always_false : std::false_type {};

};  //  class


/// <summary>
/// c++ cerealを使って構造体をバイナリデータにシリアライズ、読込。
/// </summary>
class BinarySerializer {
public:
    /// <summary>
    /// 構造体をバイナリに保存する。
    /// binファイルにしてフォルダ作成も自動で。
    /// </summary>
    /// <typeparam name="T"> 保存したい構造体の型 </typeparam>
    /// <param name="data"> 保存したい構造体の参照 </param>
    /// <param name="path"> 保存するフォルダのパス </param>
    template<typename T>
    static void Save(const std::vector<T>& data, const std::string& path) {
        //  今の操作ディレクトリにバイナリファイルを作成する。
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());
        std::ofstream os(path, std::ios::binary);
        cereal::BinaryOutputArchive archive(os);
        archive(data);
        std::cout << "[OK] " << path << " に保存しました (" << data.size() << " 件)" << std::endl;
    }

    /// <summary>
    /// バイナリファイルからデータを構造体にシリアライズ
    /// </summary>
    /// <typeparam name="T"> 読み込みたい構造体の型 </typeparam>
    /// <param name="path"> 読み込むバイナリデータのパス </param>
    /// <returns> 読み込んだデータのコレクション </returns>
    template<typename T>
    static std::vector<T> Load(const std::string& path) {
        std::ifstream is(path, std::ios::binary);
        cereal::BinaryInputArchive archive(is);
        std::vector<T> data;
        archive(data);
        return data;
    }
};


//	読み込むデータ種類
enum class LoadMode {
    CSV,
    Binary
};

namespace sf = std::filesystem;
/// <summary>
/// csvデータを読み込んで保管する。
/// 同期処理と非同期処理を切り替えられるようにする。
/// </summary>
class GameDatabase : public Singleton<GameDatabase> {
    friend class Singleton<GameDatabase>;

    //	読み込み処理（CSVかどうかを判定して、CSVなら読み込む）
    void loadData(const sf::path& dataRoot, LoadMode mode);

    //	データをコレクションに追加
    template<typename T>
    void add(int id, const T& data) {
        std::lock_guard<std::mutex> lock(mutex_);	//	スレッド固定
        GetTable<T>()[id] = data;
    }

public:

    //	スレッドの終了を確実に
    virtual ~GameDatabase() {
        if (loadthread_.joinable()) loadthread_.join();
    }

    //	初期化（同期処理）
    void Initialize(const sf::path& dataRoot, LoadMode mode);

    //	初期化（非同期処理）
    void InitializeAsync(const sf::path& dataRoot, LoadMode mode);

    //　検索機能して特定のデータを取得
    //	型によって別々のメソッドとして扱われるからデータ種類の混在はない。
    template<typename T>
    std::unordered_map<int, T>& GetTable() {
        static std::unordered_map<int, T> table;
        return table;
    }	//	gettable

    //	特定の型のコレクションを返す
    //	std::optionalが直接constを持てないのでstd::reference_wrapperでラップします。
    template<typename T>
    std::optional<std::reference_wrapper<const T>> Get(int id) {
        //	プレイヤーのインベントリなどではなく初期化以降に読み込んだら書き換えることがないので
        //	スレッド固定はするだけコストの無駄。
        //	可読性が終わってしまうので共通処理もデバックリリースで書き直してますが可読性のためなのでコード読んだ人は突っ込まないでください。
#ifdef _DEBUG
            //	テーブルの取得
        auto& table = GetTable<T>();
        auto it = table.find(id);
        //	デバック中は中身なかったら例外を投げます。
        if (it == table.end()) {
            throw std::out_of_range("[GameDatabase] 範囲外アクセス: ID " + std::to_string(id) + "型：" + typeid(T).name());
        }	//	if

#else 

        auto& table = GetTable<T>();
        auto it = table.find(id);
        //	リリースは中身がないことを明示的にします。
        if (it == table.end()) {
            return std::nullopt;
        } // if

#endif // _DEBUG

            //	戻す
        return it->second;
    }	//	Get

private:
    //	読み込み用スレッド
    std::thread loadthread_;

    //	スレッド固定（constメソッドでスレッド固定できるようにmutable）
    mutable std::mutex mutex_;

    //	読み込み終了フラグ
    bool isLoaded_ = false;

};	//	class 

