#ifndef CONVERTERFACTORY_H
#define CONVERTERFACTORY_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Converter.h"

class ConverterCreator {
public:
	virtual std::unique_ptr<Converter> createConverter(const std::vector<std::string>& args) = 0;
	virtual std::string getConverterName() const = 0;
	virtual ~ConverterCreator() = default;
};

class GainConverterCreator : public ConverterCreator {
public:
	std::unique_ptr<Converter> createConverter(const std::vector<std::string>& args) override;
	std::string getConverterName() const override;
};

class MuteConverterCreator : public ConverterCreator {
public:
	std::unique_ptr<Converter> createConverter(const std::vector<std::string>& args) override;
	std::string getConverterName() const override;
};

class MixConverterCreator : public ConverterCreator {
public:
	std::unique_ptr<Converter> createConverter(const std::vector<std::string>& args) override;
	std::string getConverterName() const override;
};

class ConverterFactory {
private:
	std::unordered_map<std::string, std::unique_ptr<ConverterCreator>> creators;
	void registerCreator(std::unique_ptr<ConverterCreator> creator);
public:
	ConverterFactory();
	std::unique_ptr<Converter> createConverter(
		const std::string& converterName,
		const std::vector<std::string>& args);

	bool hasConverter(const std::string& name) const;
};

#endif