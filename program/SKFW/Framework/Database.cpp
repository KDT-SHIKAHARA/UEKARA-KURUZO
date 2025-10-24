#include<SKFW/Framework/Database.hpp>

void GameDatabase::loadData(const sf::path& dataRoot, LoadMode mode)
{
	//	＜ディレクトリ名、読込メソッド＞でどのディレクトリに対してどの処理を行うかのテーブル作成
	std::unordered_map<std::string, std::function<void(const sf::path&)>> typeLoaders;

	//	型ごとの読み込み処理
	//	デバック中 csv読込、バイナリ書き出し
	//	リリース中 バイナリ読込

	// ================== テストデータ ===================
	//typeLoaders["Test"] = [this, debugMode](const sf::path& filepath) {
	//	auto binpath = filepath;
	//	binpath.replace_extension(".bin");
	//	if (debugMode) {
	//		auto list = CSVLoader::Load<TestData>(filepath.string());
	//		for (auto& d : list) this->add<TestData>(d.id, d);
	//		BinarySerializer::Save(list, binpath.string());
	//	}
	//	else {
	//		auto list = BinarySerializer::Load<TestData>(binpath.string());
	//		for (auto d : list) this->add<TestData>(d.id, d);
	//	}
	//};



	//	========== ディレクトリ捜査 ==========
	for (const auto& entry : sf::recursive_directory_iterator(dataRoot)) {
		if (!entry.is_regular_file())continue;	//	通常ファイル以外(ディレクトリ、リンクなど)はスキップ
		if (entry.path().extension() != ".csv") continue;	//	csv拡張子以外はスキップ

		//	親ディレクトリ名で型を判別
		std::string typeName = entry.path().parent_path().filename().string();

		//	ディレクトリ名で検索
		auto it = typeLoaders.find(typeName);
		if (it != typeLoaders.end()) {
			//	読込メソッド実行
			it->second(entry.path());

#ifdef _DEBUG
			std::ostringstream oss;
			oss << "[GameDatabase] ロード完了; " << entry.path().string() << std::endl;
			std::cout << oss.str();
#endif // !_DEBUG

			//	コンソールにログ表示

		}
	}

}

/// <summary>
/// 同期処理
/// 指定したディレクトリの.csv拡張子の
/// </summary>
/// <param name="dataRooot"></param>
/// <param name="debugMove"></param>
void GameDatabase::Initialize(const sf::path& dataRoot, LoadMode mode)
{
	loadData(dataRoot, mode);
	isLoaded_ = true;
}

void GameDatabase::InitializeAsync(const sf::path& dataRoot, LoadMode mode)
{

}

